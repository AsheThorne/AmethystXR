#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "common/enums.h"

// ----------------------------------------- //
// External Function Declarations
// ----------------------------------------- //
extern "C" {
    /// Check if the application is running
    /// @return True if the application is running
    AXR_API bool axrApplicationIsRunning();
    /// Start a new frame.
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_APPLICATION_CLOSED if the main loop should exit and program should close.
    AXR_API AxrResult axrApplicationStartNewFrame();
}
