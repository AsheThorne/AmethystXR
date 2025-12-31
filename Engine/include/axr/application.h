#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// External Function Declarations
// ----------------------------------------- //
extern "C" {
    /// Check if the application is running
    /// @return True if the application is running
    AXR_API bool axrApplicationIsRunning();
    /// Process all application events
    /// @return False if the application closed and the game loop should exit
    AXR_API bool axrApplicationProcessEvents();
}
