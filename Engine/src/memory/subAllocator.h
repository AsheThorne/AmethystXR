#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "types.h"

#include <cstddef>
#include <cstdint>

/// Base generic sub allocator to inherit from
class AxrSubAllocator {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default constructor
    AxrSubAllocator();
    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocator A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrSubAllocator(void* memory, size_t size, const AxrDeallocateBlock& deallocator);
    /// Copy Constructor
    /// @param src Source a to copy from
    AxrSubAllocator(const AxrSubAllocator& src) = delete;
    /// Move Constructor
    /// @param src Source a to move from
    AxrSubAllocator(AxrSubAllocator&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrSubAllocator();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source a to copy from
    AxrSubAllocator& operator=(const AxrSubAllocator& src) = delete;
    /// Move Assignment Operator
    /// @param src Source a to move from
    AxrSubAllocator& operator=(AxrSubAllocator&& src) noexcept;

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
