#pragma once

// ----------------------------------------- //
// C Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrWindowSystem Handle
typedef class AxrWindowSystem* AxrWindowSystem_T;

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the window system
struct AxrWindowSystemConfig {
    uint32_t Width;
    uint32_t Height;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Check if the window is open
    /// @param windowSystem The window system to use
    /// @returns True if the window is open
    AXR_API bool axrWindowSystemIsWindowOpen(AxrWindowSystem_T windowSystem);
    /// Open the window
    /// @param windowSystem The window system to use
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrWindowSystemOpenWindow(AxrWindowSystem_T windowSystem);
    /// Signal that we want to close the window
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemCloseWindow(AxrWindowSystem_T windowSystem);
}
