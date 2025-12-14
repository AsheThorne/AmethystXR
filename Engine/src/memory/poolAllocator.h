#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "axr/logging.h"
#include "types.h"

#include <cassert>

/// Pool Allocator
/// @tparam Type The type of object this pool holds
template<typename Type>
class AxrPoolAllocator;

/// Pool Allocator where T is big enough to fit a 'next' pointer
/// @tparam Type The type of object this pool holds
template<typename Type>
    requires(sizeof(Type) >= sizeof(void*))
class AxrPoolAllocator<Type> {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocate A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrPoolAllocator(void* memory, const size_t size, const AxrDeallocate& deallocate) {
        assert(size % sizeof(Type) == 0);

        m_MainMemoryDeallocator = deallocate;
        m_Memory = memory;
        m_MemoryCapacity = size;
        m_ChunkCapacity = size / sizeof(Type);
        m_FreeChunksHead = static_cast<Chunk*>(m_Memory);

        chainAllChunks();
    }

    /// Copy Constructor
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator(const AxrPoolAllocator& src) = delete;

    /// Move Constructor
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator(AxrPoolAllocator&& src) noexcept {
        m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
        m_FreeChunksHead = src.m_FreeChunksHead;
        m_Memory = src.m_Memory;
        m_MemoryCapacity = src.m_MemoryCapacity;
        m_ChunkCapacity = src.m_ChunkCapacity;
        m_UsedChunkCount = src.m_UsedChunkCount;

        src.m_MainMemoryDeallocator = {};
        src.m_FreeChunksHead = {};
        src.m_Memory = {};
        src.m_MemoryCapacity = {};
        src.m_ChunkCapacity = {};
        src.m_UsedChunkCount = {};
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

            m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
            m_FreeChunksHead = src.m_FreeChunksHead;
            m_Memory = src.m_Memory;
            m_MemoryCapacity = src.m_MemoryCapacity;
            m_ChunkCapacity = src.m_ChunkCapacity;
            m_UsedChunkCount = src.m_UsedChunkCount;

            src.m_MainMemoryDeallocator = {};
            src.m_FreeChunksHead = {};
            src.m_Memory = {};
            src.m_MemoryCapacity = {};
            src.m_ChunkCapacity = {};
            src.m_UsedChunkCount = {};
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory for AxrPoolAllocator. "
    /// Allocate a new chunk from the pool
    /// @param chunk Output allocated chunk
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there aren't any free chunks in the pool.
    [[nodiscard]] AxrResult allocate(Type*& chunk) {
        if (m_FreeChunksHead == nullptr) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of chunks to allocate.");
            return AXR_ERROR_OUT_OF_MEMORY;
        }

        chunk = reinterpret_cast<Type*>(m_FreeChunksHead);
        m_FreeChunksHead = m_FreeChunksHead->Next;

        std::memset(chunk, 0, sizeof(Type));

        m_UsedChunkCount++;
        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Return the given chunk back to the pool
    /// @param chunk Chunk to return to the pool
    void deallocate(Type*& chunk) {
        auto chunkCasted = reinterpret_cast<Chunk*>(chunk);

        chunkCasted->Next = m_FreeChunksHead;
        m_FreeChunksHead = chunkCasted;
        chunk == nullptr;

        m_UsedChunkCount--;
    }

    /// Clear the pool and mark all chunks as free
    void clear() {
        m_UsedChunkCount = 0;
        m_FreeChunksHead = static_cast<Chunk*>(m_Memory);
        chainAllChunks();
    }

    /// Get the allocator's capacity
    /// @return The allocator's capacity
    [[nodiscard]] size_t memoryCapacity() const {
        return m_MemoryCapacity;
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

    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const {
        return m_UsedChunkCount == 0;
    }

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// A chunk of free memory with a pointer to the next free chunk
    struct Chunk {
        Chunk* Next{};
    };

    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrDeallocate m_MainMemoryDeallocator{};
    Chunk* m_FreeChunksHead{};
    void* m_Memory{};
    size_t m_MemoryCapacity{};
    size_t m_ChunkCapacity{};
    size_t m_UsedChunkCount{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        if (m_Memory != nullptr) {
            if (m_MainMemoryDeallocator) {
                m_MainMemoryDeallocator(m_Memory);
            } else {
                axrLogWarning(
                    "Memory leak detected inside AxrPoolAllocator. Failed to deallocate a block of memory. No "
                    "deallocator available.");
            }
        }
        m_FreeChunksHead = {};
        m_MemoryCapacity = {};
        m_ChunkCapacity = {};
        m_UsedChunkCount = {};
        m_MainMemoryDeallocator.reset();
    }

    /// Chain together all chunks, marking them all as free to use
    void chainAllChunks() {
        assert(m_Memory != nullptr);

        auto currentChunk = static_cast<Chunk*>(m_Memory);
        for (int i = 0; i < m_ChunkCapacity - 1; ++i) {
            currentChunk->Next = reinterpret_cast<Chunk*>(reinterpret_cast<Type*>(currentChunk) + 1);
            currentChunk = currentChunk->Next;
        }
        currentChunk->Next = nullptr;
    }
};

// TODO: Implement
/// Pool Allocator where T is smaller than a pointer
/// @tparam Type The type of object this pool holds
template<typename Type>
    requires(sizeof(Type) < sizeof(void*))
class AxrPoolAllocator<Type> {
public:
};
