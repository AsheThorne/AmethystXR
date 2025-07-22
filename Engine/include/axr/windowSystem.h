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
// Structs
// ----------------------------------------- //

/// Config for the window system
struct AxrWindowSystemConfig {
    uint32_t Width;
    uint32_t Height;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrWindowSystem Handle
typedef class AxrWindowSystem* AxrWindowSystem_T;
/// const AxrWindowSystem Handle
typedef const AxrWindowSystem* AxrWindowSystemConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Check if the window system is valid for use
    /// @param windowSystem The widow system to use
    /// @returns True if the window system is valid for use
    AXR_API bool axrWindowSystemIsValid(AxrWindowSystemConst_T windowSystem);
    /// Check if the window is open
    /// @param windowSystem The window system to use
    /// @returns True if the window is open
    AXR_API bool axrWindowSystemIsWindowOpen(AxrWindowSystemConst_T windowSystem);
    /// Open the window
    /// @param windowSystem The window system to use
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrWindowSystemOpenWindow(AxrWindowSystem_T windowSystem);
    /// Signal that we want to close the window
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemCloseWindow(AxrWindowSystem_T windowSystem);
    /// Check if the cursor is hidden
    /// @param windowSystem The window system to use
    /// @returns True if the cursor is hidden
    AXR_API bool axrWindowSystemIsCursorHidden(AxrWindowSystemConst_T windowSystem);
    /// Unhide the cursor
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemShowCursor(AxrWindowSystem_T windowSystem);
    /// Hide the cursor
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemHideCursor(AxrWindowSystem_T windowSystem);
    /// Check if the cursor's position is locked
    /// @param windowSystem The window system to use
    /// @returns True if the cursor's position is locked
    AXR_API bool axrWindowSystemIsCursorLocked(AxrWindowSystemConst_T windowSystem);
    /// Lock the cursor's position
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemLockCursor(AxrWindowSystem_T windowSystem);
    /// Unlock the cursor's position
    /// @param windowSystem The window system to use
    AXR_API void axrWindowSystemUnlockCursor(AxrWindowSystem_T windowSystem);
}
