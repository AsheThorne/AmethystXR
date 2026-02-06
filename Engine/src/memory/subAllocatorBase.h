#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "types.h"

#include <cstddef>
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
