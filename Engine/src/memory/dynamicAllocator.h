#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../common/containers/redBlackTree_pool.h"
#include "../common/handle.h"
#include "subAllocatorBase.h"

// TODO (Ashe): This is implemented as a linked list dynamic allocator. Look into making it a Red Black Tree
//  (Ref = https://github.com/mtrebi/memory-allocators?tab=readme-ov-file#red-black-tree-data-structure) dynamic
//  allocator for better performance

/// Dynamic allocator
class AxrDynamicAllocator : public AxrSubAllocatorBase {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    using HandlesTree_T = AxrRedBlackTree_Pool<uintptr_t>;

    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// Describes the data item that is immediately after this header
    struct DataHeader {
        /// The size of the data following this header. Does NOT include the size of the header itself
        size_t Size;
        uint8_t Alignment;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrDynamicAllocator();
    /// Constructor
    /// @param memoryBlock Memory block to use
    /// @param handlesAllocator The handles allocator to use
    /// used to store the handles. Use `getHandlesMemoryBlockCapacity()` and add the result to the `size` parameter.
    AxrDynamicAllocator(const AxrMemoryBlock& memoryBlock, AxrPoolAllocator<HandlesTree_T::Node>* handlesAllocator);
    /// Copy Constructor
    /// @param src Source AxrDynamicAllocator to copy from
    AxrDynamicAllocator(const AxrDynamicAllocator& src) = delete;
    /// Move Constructor
    /// @param src Source AxrDynamicAllocator to move from
    AxrDynamicAllocator(AxrDynamicAllocator&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrDynamicAllocator();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrDynamicAllocator to copy from
    AxrDynamicAllocator& operator=(const AxrDynamicAllocator& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrDynamicAllocator to move from
    AxrDynamicAllocator& operator=(AxrDynamicAllocator&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Allocate a new memory block
    /// @param size Size in bytes for how much memory to allocate
    /// @param alignment Memory alignment
    /// @param handle Output allocated memory handle
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space for the requested memory.
    [[nodiscard]] AxrResult allocateBlock(size_t size,
                                          uint8_t alignment,
                                          AxrHandle<void>& handle,
                                          bool zeroOutMemory = false);

    /// Allocate new memory
    /// @tparam Type The memory data type
    /// @param size The number of data items of type `Type` to store in memory
    /// @param handle Output allocated memory handle
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space for the requested memory.
    template<typename Type>
    [[nodiscard]] AxrResult allocate(const size_t size, AxrHandle<Type>& handle, const bool zeroOutMemory = false) {
        return allocateBlock(sizeof(Type) * size,
                             alignof(Type),
                             reinterpret_cast<AxrHandle<void>&>(handle),
                             zeroOutMemory);
    }
    
    /// Reallocate an existing memory block
    /// @param size The new size in bytes for how much memory to reallocate for
    /// @param alignment Memory alignment
    /// @param handle Input/Output allocated memory handle
    /// @param zeroOutNewMemory If true, all newly allocated memory will be zeroed out. This doesn't affect memory from
    /// the existing allocation.
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space for the requested memory.
    [[nodiscard]] AxrResult reallocateBlock(size_t size,
                                            uint8_t alignment,
                                            AxrHandle<void>& handle,
                                            bool zeroOutNewMemory = false);
    /// Reallocate an existing memory block
    /// @tparam Type The memory data type
    /// @param size The new number of data items of type `Type` to store in memory
    /// @param handle Input/Output allocated memory handle
    /// @param zeroOutNewMemory If true, all newly allocated memory will be zeroed out. This doesn't affect memory from
    /// the existing allocation.
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space for the requested memory.
    template<typename Type>
    [[nodiscard]] AxrResult reallocate(const size_t size,
                                       AxrHandle<Type>& handle,
                                       const bool zeroOutNewMemory = false) {
        return reallocateBlock(sizeof(Type) * size,
                               alignof(Type),
                               reinterpret_cast<AxrHandle<void>&>(handle),
                               zeroOutNewMemory);
    }

    /// Deallocate the given handle
    /// @param handle Handle to deallocate
    void deallocateHandle(AxrHandle<void>& handle);

    /// Deallocate the given handle
    /// @param handle Handle to deallocate
    template<typename Type>
    void deallocate(AxrHandle<Type>& handle) {
        deallocateHandle(reinterpret_cast<AxrHandle<void>&>(handle));
    }

    /// Defragment the given number of data blocks
    /// @param blockCount The number of blocks to defragment
    void defragment(uint32_t blockCount);

    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const;
    /// Get the number of bytes currently in use
    /// @return The number of bytes currently in use
    [[nodiscard]] size_t size() const;

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    /// Get the peak number of bytes in use at one time
    /// @return The peak number of bytes in use at one time
    [[nodiscard]] size_t peakSize() const;
#endif

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// Memory layout looks like the following:
    /// [DataHeader][Item1][--FreeBlockHeader--][DataHeader][Item2][DataHeader][Item3][------FreeBlockHeader------]
    /// Even if there's no data in this allocator, a FreeBlockHeader is present, like so.
    /// [---------------------------------------------FreeBlockHeader---------------------------------------------]

    /// Describes the free block that this header occupies
    struct FreeBlockHeader {
        /// The size of the free block. And since this `FreeBlockHeader` occupies that space, its size is included.
        size_t Size;
        FreeBlockHeader* Next;
    };

    // ----------------------------------------- //
    // Private Data
    // ----------------------------------------- //
    HandlesTree_T m_HandlesTree{};

    /// This head will always point to the lowest address. And each next free block is ordered from lowest to highest.
    FreeBlockHeader* m_FreeBlocksHead{};
    size_t m_Size{};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    size_t m_PeakSize{};
#endif

    /// Any block must be at least the size of `FreeBlockHeader` because when this memory is freed, there needs to
    /// be enough space to insert a `FreeBlockHeader` in its place.
    /// So the smallest size a single data item can be is simply the size of `FreeBlockHeader` minus `DataHeader`
    constexpr static size_t const& m_MinDataItemSize = sizeof(FreeBlockHeader) - sizeof(DataHeader);

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Move the given AxrDynamicAllocator to this class
    /// @param src AxrDynamicAllocator to move
    void move_internal(AxrDynamicAllocator&& src);

    /// Find the first free block that has enough space for the given size
    /// @param size The minimum amount of space requested
    /// @param prevFreeBlock Output previous free block from the one returned
    /// @return A pointer to the free block to use. Or nullptr if no block was found which meets the requirements
    [[nodiscard]] FreeBlockHeader* findFreeBlock(size_t size, FreeBlockHeader*& prevFreeBlock) const;
    /// Find the adjacent free block, with a lower address, to the given address.
    /// The returned adjacent free block will always have a lower address than the given address. And the .Next pointer
    /// of the returned free block will always be the free block after the given address.
    /// @param address The address to use
    /// @return A pointer to the adjacent free block. Or nullptr no free block with a lower address was found.
    [[nodiscard]] FreeBlockHeader* findAdjacentFreeBlock(uintptr_t address) const;

    /// Starting from the given free block, recursively check for connecting free blocks that can be merged into one.
    /// Continue recursively until a free block that isn't connected the given block is found, or if there are no
    /// more free blocks to check.
    /// @param freeBlock Starting free block header
    static void mergeFreeBlocks(FreeBlockHeader* freeBlock);

    /// Defragment a single data block
    void defragment();
};
