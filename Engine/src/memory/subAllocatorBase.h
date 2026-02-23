#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "types.h"
#include "utils.h"

#include <cstdint>

/// Base generic sub allocator to inherit from
class AxrSubAllocatorBase {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default constructor
    AxrSubAllocatorBase();
    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocator A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrSubAllocatorBase(void* memory, size_t size, const AxrDeallocateBlock& deallocator);
    /// Copy Constructor
    /// @param src Source AxrSubAllocatorBase to copy from
    AxrSubAllocatorBase(const AxrSubAllocatorBase& src) = delete;
    /// Move Constructor
    /// @param src Source AxrSubAllocatorBase to move from
    AxrSubAllocatorBase(AxrSubAllocatorBase&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrSubAllocatorBase();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrSubAllocatorBase to copy from
    AxrSubAllocatorBase& operator=(const AxrSubAllocatorBase& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrSubAllocatorBase to move from
    AxrSubAllocatorBase& operator=(AxrSubAllocatorBase&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the allocator's capacity
    /// @return The allocator's capacity
    [[nodiscard]] size_t capacity() const;

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    AxrDeallocateBlock m_MainMemoryDeallocator{};
    uint8_t* m_Memory{};
    size_t m_Capacity{};

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};

/// Base generic sub allocator with type alignment to inherit from
template<typename Type>
class AxrSubAllocatorBase_Aligned : public AxrSubAllocatorBase {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrSubAllocatorBase_Aligned() = default;

    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocator A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrSubAllocatorBase_Aligned(void* memory, const size_t size, const AxrDeallocateBlock& deallocator) :
        AxrSubAllocatorBase(memory, size, deallocator) {

        m_Memory = static_cast<uint8_t*>(axrAlignMemory(m_Memory, alignof(Type)));
        m_Capacity = m_Capacity - alignof(Type);
    }

    /// Copy Constructor
    /// @param src Source AxrSubAllocatorBase_Aligned to copy from
    AxrSubAllocatorBase_Aligned(const AxrSubAllocatorBase_Aligned& src) = delete;

    /// Move Constructor
    /// @param src Source AxrSubAllocatorBase_Aligned to move from
    AxrSubAllocatorBase_Aligned(AxrSubAllocatorBase_Aligned&& src) noexcept :
        AxrSubAllocatorBase(std::move(src)) {
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrSubAllocatorBase_Aligned() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrSubAllocatorBase_Aligned to copy from
    AxrSubAllocatorBase_Aligned& operator=(const AxrSubAllocatorBase_Aligned& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrSubAllocatorBase_Aligned to move from
    AxrSubAllocatorBase_Aligned& operator=(AxrSubAllocatorBase_Aligned&& src) noexcept {
        if (this != &src) {
            cleanup();

            AxrSubAllocatorBase::operator=(std::move(src));
        }
        return *this;
    }

    /// Clean up this class
    void cleanup() {
        if (m_Memory != nullptr) {
            m_Memory = axrUnalignMemory(m_Memory);
        }

        AxrSubAllocatorBase::cleanup();
    }
};
