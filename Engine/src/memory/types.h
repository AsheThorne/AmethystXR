#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/callback.h"
#include "axr/common/enums.h"

#include <cstddef>
#include <cstdint>

// ----------------------------------------- //
// Types
// ----------------------------------------- //

/// Axr deallocate memory block function
using AxrDeallocateBlock = AxrCallback<void(void*& memory)>;

/// Axr memory block
struct AxrMemoryBlock {
    /// A pointer to the block of memory
    void* Memory{};
    /// The number of bytes the given block of memory has
    size_t Size{};
    /// A function pointer to use when we're done with the given memory block and wish to deallocate it
    AxrDeallocateBlock Deallocator{};
};
