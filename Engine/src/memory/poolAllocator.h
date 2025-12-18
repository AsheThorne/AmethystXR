#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "axr/logging.h"
#include "types.h"
#include "utils.h"

#include <cassert>

/// Pool Allocator
/// @tparam Type The type of object this pool holds
/// @tparam IsAligned Weather to align addresses optimally (Requires more memory but is better for performance)
template<typename Type, bool IsAligned>
class AxrPoolAllocator;

// ---------------------------------------------------------------------------------- //
//                       AxrPoolAllocator - Type fits pointer                         //
// ---------------------------------------------------------------------------------- //

/// Pool Allocator where T is big enough to fit a 'next' pointer
/// @tparam Type The type of object this pool holds
/// @tparam IsAligned Weather to align addresses optimally (Requires more memory but is better for performance)
template<typename Type, bool IsAligned>
    requires((IsAligned ? sizeof(Type) + alignof(Type) : sizeof(Type)) >= sizeof(void*))
class AxrPoolAllocator<Type, IsAligned> {
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
        assert(size % getChunkSize() == 0);

        m_MainMemoryDeallocator = deallocate;
        m_Memory = static_cast<Type*>(memory);
        m_MemoryCapacity = size;
        m_ChunkCapacity = size / getChunkSize();
        m_FreeChunksHead = reinterpret_cast<Chunk*>(m_Memory);

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
    /// Allocate new memory from the pool
    /// @param memory Output allocated memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't any free memory in the pool.
    [[nodiscard]] AxrResult allocate(Type*& memory) {
        if (m_FreeChunksHead == nullptr) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of chunks to allocate.");
            return AXR_ERROR_OUT_OF_MEMORY;
        }

        Type* chunk = reinterpret_cast<Type*>(m_FreeChunksHead);
        m_FreeChunksHead = m_FreeChunksHead->Next;
        std::memset(chunk, 0, getChunkSize());

        if (IsAligned) {
            chunk = axrAlignMemory(chunk);
        }

        memory = chunk;
        m_UsedChunkCount++;
        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Return the given memory back to the pool
    /// @param memory Memory to return to the pool
    void deallocate(Type*& memory) {
        Type* chunk = memory;
        if (IsAligned) {
            chunk = axrUnalignMemory(chunk);
        }
        auto chunkCasted = reinterpret_cast<Chunk*>(chunk);

        chunkCasted->Next = m_FreeChunksHead;
        m_FreeChunksHead = chunkCasted;
        memory = nullptr;

        m_UsedChunkCount--;
    }

    /// Clear the pool and mark all chunks as free
    void clear() {
        m_UsedChunkCount = 0;
        m_FreeChunksHead = reinterpret_cast<Chunk*>(m_Memory);
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
    Type* m_Memory{};
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
                m_MainMemoryDeallocator(reinterpret_cast<void*&>(m_Memory));
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
        if (m_Memory == nullptr)
            return;

        auto currentChunk = reinterpret_cast<Chunk*>(m_Memory);
        for (int i = 0; i < m_ChunkCapacity - 1; ++i) {
            currentChunk->Next = reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t*>(currentChunk) + getChunkSize());
            currentChunk = currentChunk->Next;
        }
        currentChunk->Next = nullptr;
    }

    /// Get the size of a single chunk
    /// @return The size of a single chunk
    static size_t getChunkSize() {
        if (IsAligned) {
            return sizeof(Type) + alignof(Type);
        }
        return sizeof(Type);
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
/// @tparam IsAligned Weather to align addresses optimally (Requires more memory but is better for performance)
template<typename Type, bool IsAligned>
    requires((IsAligned ? sizeof(Type) + alignof(Type) : sizeof(Type)) < sizeof(void*))
class AxrPoolAllocator<Type, IsAligned> {
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

    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocate A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrPoolAllocator(void* memory, const size_t size, const AxrDeallocate& deallocate) {
        assert(size % getChunkSize() == 0);

        m_MainMemoryDeallocator = deallocate;
        m_Memory = static_cast<Type*>(memory);
        m_MemoryCapacity = size;
        m_ChunkCapacity = size / getChunkSize();
        m_FreeChunksHeadIndex = 0;

        assert(m_ChunkCapacity <= ChunkIndexTraits::Max);

        chainAllChunks();
    }

    /// Copy Constructor
    /// @param src Source AxrPoolAllocator to copy from
    AxrPoolAllocator(const AxrPoolAllocator& src) = delete;

    /// Move Constructor
    /// @param src Source AxrPoolAllocator to move from
    AxrPoolAllocator(AxrPoolAllocator&& src) noexcept {
        m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
        m_FreeChunksHeadIndex = src.m_FreeChunksHeadIndex;
        m_Memory = src.m_Memory;
        m_MemoryCapacity = src.m_MemoryCapacity;
        m_ChunkCapacity = src.m_ChunkCapacity;
        m_UsedChunkCount = src.m_UsedChunkCount;

        src.m_MainMemoryDeallocator = {};
        src.m_FreeChunksHeadIndex = {};
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
            m_FreeChunksHeadIndex = src.m_FreeChunksHeadIndex;
            m_Memory = src.m_Memory;
            m_MemoryCapacity = src.m_MemoryCapacity;
            m_ChunkCapacity = src.m_ChunkCapacity;
            m_UsedChunkCount = src.m_UsedChunkCount;

            src.m_MainMemoryDeallocator = {};
            src.m_FreeChunksHeadIndex = {};
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
    /// Allocate new memory from the pool
    /// @param memory Output allocated memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't any free memory in the pool.
    [[nodiscard]] AxrResult allocate(Type*& memory) {
        if (m_FreeChunksHeadIndex == ChunkIndexTraits::Max) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of chunks to allocate.");
            return AXR_ERROR_OUT_OF_MEMORY;
        }

        Type* chunk = ptrAt(m_FreeChunksHeadIndex);
        m_FreeChunksHeadIndex = at(m_FreeChunksHeadIndex);
        std::memset(chunk, 0, getChunkSize());

        if (IsAligned) {
            chunk = axrAlignMemory(chunk);
        }

        memory = chunk;
        m_UsedChunkCount++;
        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Return the given memory back to the pool
    /// @param memory Memory to return to the pool
    void deallocate(Type*& memory) {
        uint8_t* chunk = memory;
        if (IsAligned) {
            chunk = axrUnalignMemory(chunk);
        }
        const auto index = static_cast<ChunkIndexTraits::Type>((chunk - m_Memory) / getChunkSize());

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
    // Private Variables
    // ----------------------------------------- //
    AxrDeallocate m_MainMemoryDeallocator{};
    uint8_t* m_Memory{};
    size_t m_MemoryCapacity{};
    size_t m_ChunkCapacity{};
    size_t m_UsedChunkCount{};
    ChunkIndexTraits::Type m_FreeChunksHeadIndex{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        if (m_Memory != nullptr) {
            if (m_MainMemoryDeallocator) {
                m_MainMemoryDeallocator(reinterpret_cast<void*&>(m_Memory));
            } else {
                axrLogWarning(
                    "Memory leak detected inside AxrPoolAllocator. Failed to deallocate a block of memory. No "
                    "deallocator available.");
            }
        }
        m_FreeChunksHeadIndex = {};
        m_MemoryCapacity = {};
        m_ChunkCapacity = {};
        m_UsedChunkCount = {};
        m_MainMemoryDeallocator.reset();
    }

    /// Chain together all chunks, marking them all as free to use
    void chainAllChunks() {
        if (m_Memory == nullptr)
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
        return m_Memory + (index * getChunkSize());
    }

    /// Get the size of a single chunk
    /// @return The size of a single chunk
    static size_t getChunkSize() {
        if (IsAligned) {
            return sizeof(Type) + alignof(Type);
        }
        return sizeof(Type);
    }
};
