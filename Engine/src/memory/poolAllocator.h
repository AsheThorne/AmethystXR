#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "axr/logging.h"
#include "subAllocatorBase.h"
#include "types.h"

#include <cassert>

/// Pool Allocator
/// @tparam Type The type of object this pool holds
template<typename Type>
class AxrPoolAllocator;

// ---------------------------------------------------------------------------------- //
//                       AxrPoolAllocator - Type fits pointer                         //
// ---------------------------------------------------------------------------------- //

/// Pool Allocator where T is big enough to fit a 'next' pointer
/// @tparam Type The type of object this pool holds
template<typename Type>
    requires(sizeof(Type) + alignof(Type) >= sizeof(void*))
class AxrPoolAllocator<Type> : public AxrSubAllocatorBase_Aligned<Type> {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrPoolAllocator() = default;

    /// Constructor
    /// @param memoryBlock Memory block to use
    explicit AxrPoolAllocator(const AxrMemoryBlock& memoryBlock) :
        AxrSubAllocatorBase_Aligned<Type>(memoryBlock) {
        assert(AxrSubAllocatorBase::m_Capacity % sizeof(Type) == 0);

        m_ChunkCapacity = AxrSubAllocatorBase::m_Capacity / sizeof(Type);
        m_FreeChunksHead = reinterpret_cast<Chunk*>(AxrSubAllocatorBase::m_Memory);

        chainAllChunks();
    }

    /// Copy Constructor
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator(const AxrPoolAllocator& src) = delete;

    /// Move Constructor
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator(AxrPoolAllocator&& src) noexcept :
        AxrSubAllocatorBase_Aligned<Type>(std::move(src)) {
        m_FreeChunksHead = src.m_FreeChunksHead;
        m_ChunkCapacity = src.m_ChunkCapacity;
        m_UsedChunkCount = src.m_UsedChunkCount;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        m_PeakUsedChunkCount = src.m_PeakUsedChunkCount;
#endif

        src.m_FreeChunksHead = {};
        src.m_ChunkCapacity = {};
        src.m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        src.m_PeakUsedChunkCount = {};
#endif
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrPoolAllocator() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator& operator=(const AxrPoolAllocator& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator& operator=(AxrPoolAllocator&& src) noexcept {
        if (this != &src) {
            cleanup();

            AxrSubAllocatorBase_Aligned<Type>::operator=(std::move(src));

            m_FreeChunksHead = src.m_FreeChunksHead;
            m_ChunkCapacity = src.m_ChunkCapacity;
            m_UsedChunkCount = src.m_UsedChunkCount;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
            m_PeakUsedChunkCount = src.m_PeakUsedChunkCount;
#endif

            src.m_FreeChunksHead = {};
            src.m_ChunkCapacity = {};
            src.m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
            src.m_PeakUsedChunkCount = {};
#endif
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory for AxrPoolAllocator. "
    /// Allocate new memory from the pool
    /// @param memory Output allocated memory
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't any free memory in the pool.
    [[nodiscard]] AxrResult allocate(Type*& memory, const bool zeroOutMemory = false) {
        if (m_FreeChunksHead == nullptr) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of chunks to allocate.");
            return AXR_ERROR_OUT_OF_MEMORY;
        }

        Type* chunk = reinterpret_cast<Type*>(m_FreeChunksHead);
        m_FreeChunksHead = m_FreeChunksHead->Next;

        if (zeroOutMemory) {
            std::memset(static_cast<void*>(chunk), 0, sizeof(Type));
        }

        memory = chunk;
        ++m_UsedChunkCount;

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        if (m_UsedChunkCount > m_PeakUsedChunkCount) {
            m_PeakUsedChunkCount = m_UsedChunkCount;
        }
#endif

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Return the given memory back to the pool
    /// @param memory Memory to return to the pool
    void deallocate(Type*& memory) {
        const auto memoryAddress = reinterpret_cast<uintptr_t>(memory);
        // If the given data isn't part of the memory we manage, don't do anything with it
        if (memoryAddress < reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) ||
            memoryAddress > reinterpret_cast<uintptr_t>(AxrSubAllocatorBase::m_Memory) +
                                AxrSubAllocatorBase::m_Capacity) [[unlikely]] {
            axrLogWarning("Attempted to deallocate data that isn't from this pool allocator.");
            return;
        }

        auto chunk = reinterpret_cast<Chunk*>(memory);

        chunk->Next = m_FreeChunksHead;
        m_FreeChunksHead = chunk;
        memory = nullptr;

        m_UsedChunkCount--;
    }

    /// Clear the pool and mark all chunks as free
    void clear() {
        m_UsedChunkCount = 0;
        m_FreeChunksHead = reinterpret_cast<Chunk*>(AxrSubAllocatorBase::m_Memory);
        chainAllChunks();
    }

    /// Get the max number of chunks this allocator can hold
    /// @return The max number of chunks this allocator can hold
    [[nodiscard]] size_t chunkCapacity() const {
        return m_ChunkCapacity;
    }

    /// Get the number of chunks currently in use
    /// @return the number of chunks currently in use
    [[nodiscard]] size_t size() const {
        return m_UsedChunkCount;
    }

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    /// Get the peak number of chunks in use at one time
    /// @return the peak number of chunks in use at one time
    [[nodiscard]] size_t peakChunkCount() const {
        return m_PeakUsedChunkCount;
    }
#endif

    /// Get the empty state of the allocator
    /// @return True if the allocator is empty and all chunks are free to use
    [[nodiscard]] bool empty() const {
        return m_UsedChunkCount == 0;
    }

protected:
    // ----------------------------------------- //
    // Protected Structs
    // ----------------------------------------- //

    /// A chunk of free memory with a pointer to the next free chunk
    struct Chunk {
        Chunk* Next{};
    };

    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    Chunk* m_FreeChunksHead{};
    size_t m_ChunkCapacity{};
    size_t m_UsedChunkCount{};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    size_t m_PeakUsedChunkCount{};
#endif

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_FreeChunksHead = {};
        m_ChunkCapacity = {};
        m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        m_PeakUsedChunkCount = {};
#endif

        AxrSubAllocatorBase_Aligned<Type>::cleanup();
    }

    /// Chain together all chunks, marking them all as free to use
    void chainAllChunks() {
        if (AxrSubAllocatorBase::m_Memory == nullptr || m_ChunkCapacity == 0)
            return;

        auto currentChunk = reinterpret_cast<Chunk*>(AxrSubAllocatorBase::m_Memory);
        for (size_t i = 0; i < m_ChunkCapacity - 1; ++i) {
            // TODO: Maybe use uintptr_t instead of uint8* type.
            //  Check other places it should be replaced too
            currentChunk->Next = reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t*>(currentChunk) + sizeof(Type));
            currentChunk = currentChunk->Next;
        }
        currentChunk->Next = nullptr;
    }
};

// ---------------------------------------------------------------------------------- //
//                         AxrPoolAllocatorChunkIndexTraits                           //
// ---------------------------------------------------------------------------------- //

template<typename T>
struct AxrPoolAllocatorChunkIndexTraits;

template<typename T>
    requires(sizeof(T) <= sizeof(uint8_t))
struct AxrPoolAllocatorChunkIndexTraits<T> {
    using Type = uint8_t;
    static constexpr size_t Max = UINT8_MAX;
};

template<typename T>
    requires(sizeof(T) > sizeof(uint8_t) && sizeof(T) <= sizeof(uint16_t))
struct AxrPoolAllocatorChunkIndexTraits<T> {
    using Type = uint16_t;
    static constexpr size_t Max = UINT16_MAX;
};

template<typename T>
    requires(sizeof(T) > sizeof(uint16_t) && sizeof(T) <= sizeof(uint32_t))
struct AxrPoolAllocatorChunkIndexTraits<T> {
    using Type = uint32_t;
    static constexpr size_t Max = UINT32_MAX;
};

template<typename T>
    requires(sizeof(T) > sizeof(uint32_t) && sizeof(T) <= sizeof(uint64_t))
struct AxrPoolAllocatorChunkIndexTraits<T> {
    using Type = uint64_t;
    static constexpr size_t Max = UINT64_MAX;
};

// ---------------------------------------------------------------------------------- //
//                   AxrPoolAllocator - Type smaller than pointer                     //
// ---------------------------------------------------------------------------------- //

/// Pool Allocator where T is smaller than a pointer
/// @tparam Type The type of object this pool holds
template<typename Type>
    requires(sizeof(Type) + alignof(Type) < sizeof(void*))
class AxrPoolAllocator<Type> : public AxrSubAllocatorBase_Aligned<Type> {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Chunk index properties
    using ChunkIndexTraits = AxrPoolAllocatorChunkIndexTraits<Type>;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrPoolAllocator() = default;

    /// Constructor
    /// @param memoryBlock Memory block to use
    explicit AxrPoolAllocator(const AxrMemoryBlock& memoryBlock) :
        AxrSubAllocatorBase_Aligned<Type>(memoryBlock) {
        assert(AxrSubAllocatorBase::m_Capacity % sizeof(Type) == 0);

        m_ChunkCapacity = AxrSubAllocatorBase::m_Capacity / sizeof(Type);
        m_FreeChunksHeadIndex = 0;

        assert(m_ChunkCapacity <= ChunkIndexTraits::Max);

        chainAllChunks();
    }

    /// Copy Constructor
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator(const AxrPoolAllocator& src) = delete;

    /// Move Constructor
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator(AxrPoolAllocator&& src) noexcept :
        AxrSubAllocatorBase_Aligned<Type>(std::move(src)) {
        m_FreeChunksHeadIndex = src.m_FreeChunksHeadIndex;
        m_ChunkCapacity = src.m_ChunkCapacity;
        m_UsedChunkCount = src.m_UsedChunkCount;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        m_PeakUsedChunkCount = src.m_PeakUsedChunkCount;
#endif

        src.m_FreeChunksHeadIndex = {};
        src.m_ChunkCapacity = {};
        src.m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        src.m_PeakUsedChunkCount = {};
#endif
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrPoolAllocator() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator& operator=(const AxrPoolAllocator& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator& operator=(AxrPoolAllocator&& src) noexcept {
        if (this != &src) {
            cleanup();

            AxrSubAllocatorBase_Aligned<Type>::operator=(std::move(src));

            m_FreeChunksHeadIndex = src.m_FreeChunksHeadIndex;
            m_ChunkCapacity = src.m_ChunkCapacity;
            m_UsedChunkCount = src.m_UsedChunkCount;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
            m_PeakUsedChunkCount = src.m_PeakUsedChunkCount;
#endif

            src.m_FreeChunksHeadIndex = {};
            src.m_ChunkCapacity = {};
            src.m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
            src.m_PeakUsedChunkCount = {};
#endif
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory for AxrPoolAllocator. "
    /// Allocate new memory from the pool
    /// @param memory Output allocated memory
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't any free memory in the pool.
    [[nodiscard]] AxrResult allocate(Type*& memory, const bool zeroOutMemory = false) {
        if (m_FreeChunksHeadIndex == ChunkIndexTraits::Max) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of chunks to allocate.");
            return AXR_ERROR_OUT_OF_MEMORY;
        }

        Type* chunk = ptrAt(m_FreeChunksHeadIndex);
        m_FreeChunksHeadIndex = at(m_FreeChunksHeadIndex);

        if (zeroOutMemory) {
            std::memset(static_cast<void*>(chunk), 0, sizeof(Type));
        }

        memory = chunk;
        m_UsedChunkCount++;

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        if (m_UsedChunkCount > m_PeakUsedChunkCount) {
            m_PeakUsedChunkCount = m_UsedChunkCount;
        }
#endif

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Return the given memory back to the pool
    /// @param memory Memory to return to the pool
    void deallocate(Type*& memory) {
        const auto index = static_cast<ChunkIndexTraits::Type>((memory - AxrSubAllocatorBase::m_Memory) / sizeof(Type));

        at(index) = m_FreeChunksHeadIndex;
        m_FreeChunksHeadIndex = index;
        memory = nullptr;

        m_UsedChunkCount--;
    }

    /// Clear the pool and mark all chunks as free
    void clear() {
        m_UsedChunkCount = 0;
        m_FreeChunksHeadIndex = 0;
        chainAllChunks();
    }

    /// Get the max number of chunks this allocator can hold
    /// @return The max number of chunks this allocator can hold
    [[nodiscard]] size_t chunkCapacity() const {
        return m_ChunkCapacity;
    }

    /// Get the number of chunks currently in use
    /// @return the number of chunks currently in use
    [[nodiscard]] size_t size() const {
        return m_UsedChunkCount;
    }

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    /// Get the peak number of chunks in use at one time
    /// @return the peak number of chunks in use at one time
    [[nodiscard]] size_t peakChunkCount() const {
        return m_PeakUsedChunkCount;
    }
#endif

    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const {
        return m_UsedChunkCount == 0;
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    size_t m_ChunkCapacity{};
    size_t m_UsedChunkCount{};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    size_t m_PeakUsedChunkCount{};
#endif
    ChunkIndexTraits::Type m_FreeChunksHeadIndex{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_FreeChunksHeadIndex = {};
        m_ChunkCapacity = {};
        m_UsedChunkCount = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        m_PeakUsedChunkCount = {};
#endif

        AxrSubAllocatorBase_Aligned<Type>::cleanup();
    }

    /// Chain together all chunks, marking them all as free to use
    void chainAllChunks() {
        if (AxrSubAllocatorBase::m_Memory == nullptr)
            return;

        Type* chunk = nullptr;
        for (typename ChunkIndexTraits::Type i = 0; i < m_ChunkCapacity - 1; ++i) {
            chunk = ptrAt(i);
            *chunk = i + 1;
        }
        chunk = ptrAt(m_ChunkCapacity - 1);
        *chunk = ChunkIndexTraits::Max;
    }

    /// Get the data within memory at the given index
    /// @param index Item index
    Type at(ChunkIndexTraits::Type index) const {
        return *ptrAt(index);
    }

    /// Get the data within memory at the given index
    /// @param index Item index
    Type& at(ChunkIndexTraits::Type index) {
        return *ptrAt(index);
    }

    /// Get the data within memory at the given index
    /// @param index Item index
    Type* ptrAt(ChunkIndexTraits::Type index) const {
        return AxrSubAllocatorBase::m_Memory + (index * sizeof(Type));
    }
};
