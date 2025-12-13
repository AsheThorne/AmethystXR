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

/// Axr allocate memory function
using AxrAllocate = AxrCallback<AxrResult(std::size_t size, void*& memory)>;

/// Axr deallocate memory function
using AxrDeallocate = AxrCallback<void(void*& memory)>;
