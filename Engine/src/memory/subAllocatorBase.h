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
    /// @param memoryBlock memory block to use
    explicit AxrSubAllocatorBase(const AxrMemoryBlock& memoryBlock);
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
    AxrDeallocateBlock m_Deallocator{};
    void* m_Memory{};
    size_t m_Capacity{};

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
    
    /// Move the given AxrSubAllocatorBase to this class
    /// @param src AxrSubAllocatorBase to move
    void move_internal(AxrSubAllocatorBase&& src);
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
    /// @param memoryBlock memory block to use
    explicit AxrSubAllocatorBase_Aligned(const AxrMemoryBlock& memoryBlock) :
        AxrSubAllocatorBase(memoryBlock) {
        m_Memory = axrAlignMemory(m_Memory, alignof(Type));
        m_Capacity = m_Capacity - alignof(Type);

        assert(m_Capacity != 0);
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

protected:
    // ----------------------------------------- //
    // Protected functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        if (m_Memory != nullptr) {
            m_Memory = axrUnalignMemory(m_Memory);
        }

        AxrSubAllocatorBase::cleanup();
    }
};
