// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "dynamicAllocator.h"

#include "axr/logging.h"
#include "poolAllocator.h"
#include "utils.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrDynamicAllocator::AxrDynamicAllocator() = default;

AxrDynamicAllocator::AxrDynamicAllocator(const AxrMemoryBlock& memoryBlock,
                                         AxrPoolAllocator<HandlesTree_T::Node>* handlesAllocator) :
    AxrSubAllocatorBase(memoryBlock) {
    m_HandlesTree = HandlesTree_T(handlesAllocator);

    m_FreeBlocksHead = static_cast<FreeBlockHeader*>(AxrSubAllocatorBase::m_Memory);
    *m_FreeBlocksHead = FreeBlockHeader{
        .Size = AxrSubAllocatorBase::m_Capacity,
        .Next = nullptr,
    };
}

AxrDynamicAllocator::AxrDynamicAllocator(AxrDynamicAllocator&& src) noexcept :
    AxrSubAllocatorBase(std::move(src)) {
    move_internal(std::move(src));
}

AxrDynamicAllocator::~AxrDynamicAllocator() {
    cleanup();
}

AxrDynamicAllocator& AxrDynamicAllocator::operator=(AxrDynamicAllocator&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrSubAllocatorBase::operator=(std::move(src));

        move_internal(std::move(src));
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocator block for dynamic allocator. "
AxrResult AxrDynamicAllocator::allocateBlock(const size_t size,
                                             const uint8_t alignment,
                                             AxrHandle<void>& handle,
                                             const bool zeroOutMemory) {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    size_t blockSize = std::max(m_MinDataItemSize, size) + alignment + sizeof(DataHeader);

    FreeBlockHeader* previousFreeBlock = nullptr;
    FreeBlockHeader* freeBlock = findFreeBlock(blockSize, previousFreeBlock);

    if (freeBlock == nullptr && blockSize <= AxrSubAllocatorBase::m_Capacity - m_Size) [[unlikely]] {
        // If no free block was found, but the `blockSize` is less than or equal to the total amount of memory we have
        // spare, then it means we have the space, it's just fragmented. So instead of returning that we don't have the
        // memory, instead, defragment as little as possible until the free block is big enough.
        while (freeBlock == nullptr) {
            defragment();

            freeBlock = findFreeBlock(blockSize, previousFreeBlock);
        }

        axrLogWarning("Forced into defragmenting while allocating a memory block of size {}.", blockSize);
    } else if (freeBlock == nullptr) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a free block of memory for the requested size.");
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    // Copy the free block header
    const FreeBlockHeader originalFreeBlockHeader = *freeBlock;
    const auto originalFreeBlockAddress = reinterpret_cast<uintptr_t>(freeBlock);

    freeBlock->Size -= blockSize;
    // If there isn't enough space for a new `FreeBlockHeader` to be inserted at the end of this data block, then just
    // allocate the entire block. We can't keep track of the new free block anyway.
    // Yes, it does mean that we could end up giving extra space to many blocks of data which could theoretically be
    // combined into a single free block. But that adds more complexity than it's worth for a few extra bytes.
    if (freeBlock->Size < sizeof(FreeBlockHeader)) {
        blockSize += freeBlock->Size;
        freeBlock->Size = 0;
    }

    // ---- Allocate Memory ----

    const auto dataHeader = reinterpret_cast<DataHeader*>(freeBlock);
    // NOTE (Ashe): This header isn't aligned to an optimal address. Don't know how much that matters though.
    *dataHeader = DataHeader{
        .Size = blockSize - sizeof(DataHeader),
        .Alignment = alignment,
    };

    auto allocatedBlock = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(dataHeader) + sizeof(DataHeader));

    if (zeroOutMemory) {
        std::memset(allocatedBlock, 0, blockSize - sizeof(DataHeader));
    }

    allocatedBlock = axrAlignMemory(allocatedBlock, alignment);

    const HandlesTree_T::Node::Iterator allocatedBlockHandleNode = m_HandlesTree.insert(
        reinterpret_cast<uintptr_t>(allocatedBlock));
    if (allocatedBlockHandleNode == m_HandlesTree.end()) {
        *reinterpret_cast<FreeBlockHeader*>(originalFreeBlockAddress) = originalFreeBlockHeader;
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create handle.");
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    // ---- Create new free block ----

    if (originalFreeBlockHeader.Size - blockSize == 0) {
        // There's no extra space to create a new free block, just connect the previous free block with the next free
        // block

        if (previousFreeBlock == nullptr) {
            m_FreeBlocksHead = originalFreeBlockHeader.Next;
        } else {
            previousFreeBlock->Next = originalFreeBlockHeader.Next;
        }
    } else {
        // We don't need to check if there's enough here for the `FreeBlockHeader`. We did that earlier. If we got here,
        // there's room.

        const auto newFreeBlockAddress = reinterpret_cast<FreeBlockHeader*>(originalFreeBlockAddress + blockSize);
        *newFreeBlockAddress = originalFreeBlockHeader;
        newFreeBlockAddress->Size -= blockSize;

        if (previousFreeBlock == nullptr) {
            m_FreeBlocksHead = newFreeBlockAddress;
        } else {
            previousFreeBlock->Next = newFreeBlockAddress;
        }
    }

    // ---- Set handle ----

    AxrHandle<void>::Deallocator_T deallocatorCallback{};
    deallocatorCallback.connect<&AxrDynamicAllocator::deallocateHandle>(this);

    handle = AxrHandle(reinterpret_cast<void* const*>(&*allocatedBlockHandleNode), deallocatorCallback);

    m_Size += blockSize;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    if (m_Size > m_PeakSize) {
        m_PeakSize = m_Size;
    }
#endif

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to reallocate block for dynamic allocator. "
AxrResult AxrDynamicAllocator::reallocateBlock(const size_t size,
                                               const uint8_t alignment,
                                               AxrHandle<void>& handle,
                                               const bool zeroOutNewMemory) {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    if (handle.m_Data == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "The given handle is not allocated.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    const auto dataAddress = reinterpret_cast<uintptr_t>(*handle.m_Data);
    // If the given data isn't part of the memory we manage, don't do anything with it
    if (dataAddress < reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) ||
        dataAddress > reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) + AxrSubAllocatorBase::m_Capacity)
        [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Attempted to deallocate data that isn't from this dynamic allocator.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    const DataHeader originalDataHeader = *reinterpret_cast<DataHeader*>(
        reinterpret_cast<uintptr_t>(axrUnalignMemory(reinterpret_cast<void*>(dataAddress))) - sizeof(DataHeader));

    AxrHandle<void> newHandle;
    const AxrResult axrResult = allocateBlock(size, alignment, newHandle, zeroOutNewMemory);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate new block.");
        return axrResult;
    }

    std::memcpy(newHandle.getDataPtr(), handle.getDataPtr(), originalDataHeader.Size);

    deallocateHandle(handle);

    handle = std::move(newHandle);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrDynamicAllocator::deallocateHandle(AxrHandle<void>& handle) {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    if (handle.m_Data == nullptr) {
        return;
    }

    const auto dataAddress = reinterpret_cast<uintptr_t>(*handle.m_Data);
    // If the given data isn't part of the memory we manage, don't do anything with it
    if (dataAddress < reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) ||
        dataAddress > reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) + AxrSubAllocatorBase::m_Capacity)
        [[unlikely]] {
        axrLogWarning("Attempted to deallocate data that isn't from this dynamic allocator.");
        return;
    }

    m_HandlesTree.remove(dataAddress);

    const auto dataHeader = reinterpret_cast<DataHeader*>(
        reinterpret_cast<uintptr_t>(axrUnalignMemory(reinterpret_cast<void*>(dataAddress))) - sizeof(DataHeader));
    /// Copy the data header since we're overwriting it and don't want to lose  it's data
    const DataHeader originalDataHeader = *dataHeader;

    // ---- Replace the data with a free block ----

    const auto newFreeBlock = reinterpret_cast<FreeBlockHeader*>(dataHeader);
    *newFreeBlock = FreeBlockHeader{
        .Size = originalDataHeader.Size + sizeof(DataHeader),
        .Next = nullptr,
    };

    FreeBlockHeader* adjacentFreeBlock = findAdjacentFreeBlock(dataAddress);

    // Ensure that the order of the free blocks chain remain from lowest to highest addresses.
    if (adjacentFreeBlock == nullptr) {
        // There are no free blocks with a lower address, we need to change the head since this is the new lowest
        // free block

        if (m_FreeBlocksHead != nullptr) {
            newFreeBlock->Next = m_FreeBlocksHead;
        }

        m_FreeBlocksHead = newFreeBlock;
        mergeFreeBlocks(m_FreeBlocksHead);
    } else {
        newFreeBlock->Next = adjacentFreeBlock->Next;
        adjacentFreeBlock->Next = newFreeBlock;

        mergeFreeBlocks(adjacentFreeBlock);
    }

    handle.m_Data = nullptr;
    m_Size -= originalDataHeader.Size + sizeof(DataHeader);
}

void AxrDynamicAllocator::defragment(const uint32_t blockCount) {
    for (uint32_t i = 0; i < blockCount; ++i) {
        defragment();
    }
}

bool AxrDynamicAllocator::empty() const {
    return m_FreeBlocksHead != nullptr && m_FreeBlocksHead->Size == AxrSubAllocatorBase::m_Capacity;
}

size_t AxrDynamicAllocator::size() const {
    return m_Size;
}

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
size_t AxrDynamicAllocator::peakSize() const {
    return m_PeakSize;
}
#endif

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrDynamicAllocator::cleanup() {
    m_HandlesTree = {};
    m_FreeBlocksHead = {};
    m_Size = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = {};
#endif

    AxrSubAllocatorBase::cleanup();
}

void AxrDynamicAllocator::move_internal(AxrDynamicAllocator&& src) {
    // Please note that we aren't moving the base class. That should be done before calling this function because
    // depending on how it's done, it changes if we call the base move constructor or move assignment operator.

    m_HandlesTree = std::move(src.m_HandlesTree);

    m_FreeBlocksHead = src.m_FreeBlocksHead;
    m_Size = src.m_Size;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = src.m_PeakSize;
#endif

    src.m_FreeBlocksHead = {};
    src.m_Size = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    src.m_PeakSize = {};
#endif
}

AxrDynamicAllocator::FreeBlockHeader* AxrDynamicAllocator::findFreeBlock(const size_t size,
                                                                         FreeBlockHeader*& prevFreeBlock) const {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    if (m_FreeBlocksHead == nullptr) [[unlikely]] {
        // There are no free blocks and all memory is in use
        return nullptr;
    }

    // Find the next free block that is big enough for the requested size
    FreeBlockHeader* previousFreeBlock = nullptr;
    FreeBlockHeader* currentFreeBlock = m_FreeBlocksHead;
    while (currentFreeBlock->Size < size) {
        if (currentFreeBlock->Next == nullptr) {
            // Failed to find any free block that is big enough
            return nullptr;
        }

        previousFreeBlock = currentFreeBlock;
        currentFreeBlock = currentFreeBlock->Next;
    }

    prevFreeBlock = previousFreeBlock;
    return currentFreeBlock;
}

AxrDynamicAllocator::FreeBlockHeader* AxrDynamicAllocator::findAdjacentFreeBlock(const uintptr_t address) const {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    if (m_FreeBlocksHead == nullptr) [[unlikely]] {
        // There are no free blocks and all memory is in use
        return nullptr;
    }

    if (reinterpret_cast<uintptr_t>(m_FreeBlocksHead) > address) {
        // There are no free blocks with a lower address than the given `address`
        return nullptr;
    }

    FreeBlockHeader* currentFreeBlock = m_FreeBlocksHead;
    while (currentFreeBlock->Next != nullptr && reinterpret_cast<uintptr_t>(currentFreeBlock->Next) < address) {
        currentFreeBlock = currentFreeBlock->Next;
    }

    return currentFreeBlock;
}

void AxrDynamicAllocator::mergeFreeBlocks(FreeBlockHeader* freeBlock) {
    assert(freeBlock != nullptr);

    // Continue to merge free blocks if they are connected
    while (freeBlock->Next != nullptr &&
           reinterpret_cast<uintptr_t>(freeBlock) + freeBlock->Size == reinterpret_cast<uintptr_t>(freeBlock->Next)) {
        freeBlock->Size += freeBlock->Next->Size;
        freeBlock->Next = freeBlock->Next->Next;
    }
}

#define AXR_FUNCTION_FAILED_STRING "Failed to defragment dynamic allocator. "
void AxrDynamicAllocator::defragment() {
    // This should never be null. The only time it's null is if this allocator is empty and wasn't given any data to
    // manage. In such case, we shouldn't be calling this function
    assert(AxrSubAllocatorBase::m_Memory != nullptr);

    if (m_FreeBlocksHead == nullptr) [[unlikely]] {
        // There are no free blocks and all memory is in use
        return;
    }

    if (m_FreeBlocksHead->Next == nullptr) {
        // There are no gaps in free memory, nothing to defragment
        return;
    }

    // Defragment a single block:
    // [DataHeader1][Item1][---Free Block---][DataHeader2][Item2][---Free Block---]
    // We want to move DataHeader2 and Item2, to where the free block is, and shift the free block to after Item2.

    const auto dataHeader = reinterpret_cast<DataHeader*>(reinterpret_cast<uintptr_t>(m_FreeBlocksHead) +
                                                          m_FreeBlocksHead->Size);

    // Find the next address that is after the `dataHeader`. Since the data after the header is aligned, we don't know
    // exactly what the address of it is. We do know that it will be after the address of the header though. So we just
    // grab the next address after the header.
    const HandlesTree_T::Node::Iterator dataAddressIterator = m_HandlesTree.findNextLargest(
        reinterpret_cast<uintptr_t>(dataHeader));
    if (dataAddressIterator == m_HandlesTree.end()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find the address for the next block to defragment.");
        return;
    }

    // If a variable has "block" in its name, it's referring to the entire block, meaning the header and data together.
    const DataHeader originalDataHeader = *dataHeader;
    const FreeBlockHeader originalFreeBlockHeader = *m_FreeBlocksHead;

    const auto oldDataAddress = *dataAddressIterator;
    const auto newDataBlockAddress = reinterpret_cast<uintptr_t>(m_FreeBlocksHead);
    const uintptr_t newDataAddress = axrAlignAddress(newDataBlockAddress + sizeof(DataHeader),
                                                     originalDataHeader.Alignment);
    const size_t dataBlockSize = originalDataHeader.Size + sizeof(DataHeader);
    const size_t dataSize = originalDataHeader.Size - originalDataHeader.Alignment;

    const uintptr_t newFreeBlockAddress = newDataBlockAddress + dataBlockSize;

    // First, move just the data that was allocated, we'll do the headers after. Since we know their data type, we can
    // easily just copy them so we don't need to worry about data getting overwritten. We can't do it the other way
    // around because we can't copy the data as easily.
    // We don't just copy the entire block (header and data) because the data is aligned to a specific address. If we
    // moved the whole block, we'd need to move just the data portion of it anyway to realign it.

    memmove(reinterpret_cast<void*>(newDataAddress), reinterpret_cast<void*>(oldDataAddress), dataSize);
    *reinterpret_cast<DataHeader*>(newDataBlockAddress) = originalDataHeader;
    // Don't need to change anything about the free block data. The size and .Next pointer wouldn't change
    *reinterpret_cast<FreeBlockHeader*>(newFreeBlockAddress) = originalFreeBlockHeader;

    m_FreeBlocksHead = reinterpret_cast<FreeBlockHeader*>(newFreeBlockAddress);

    mergeFreeBlocks(m_FreeBlocksHead);

    m_HandlesTree.replace(oldDataAddress, newDataAddress);
}
#undef AXR_FUNCTION_FAILED_STRING
