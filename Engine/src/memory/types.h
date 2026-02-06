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
