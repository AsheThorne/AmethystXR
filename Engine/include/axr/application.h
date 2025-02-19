#pragma once

// ----------------------------------------- //
// C Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/windowSystem.h"
#include "axr/graphicsSystem.h"
#include "axr/assets.h"

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrApplication Handle
typedef class AxrApplication* AxrApplication_T;

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the AxrApplication
struct AxrApplicationConfig {
    const char* ApplicationName;
    uint32_t ApplicationVersion;
    AxrWindowSystemConfig WindowSystemConfig;
    AxrGraphicsSystemConfig GraphicsSystemConfig;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Create an AxrApplication
    /// @param config A handle to the application config
    /// @returns The created AxrApplication
    AXR_API AxrApplication_T axrCreateApplication(const AxrApplicationConfig* config);
    /// Destroy the given AxrApplication
    /// @param app The AxrApplication to destroy
    AXR_API void axrDestroyApplication(AxrApplication_T* app);

    /// Set up the given axrApplication
    /// @param app The AxrApplication to use
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrApplicationSetup(AxrApplication_T app);
    /// Check if the application is running
    /// @param app The AxrApplication to use
    /// @returns True if the application is running
    AXR_API bool axrApplicationIsRunning(AxrApplication_T app);
    /// Process application events
    /// @param app The AxrApplication to use
    AXR_API void axrApplicationProcessEvents(AxrApplication_T app);

    /// Get the axrApplication window system
    /// @param app The AxrApplication to use
    /// @returns A handle to the window system
    AXR_API AxrWindowSystem_T axrApplicationGetWindowSystem(AxrApplication_T app);
    /// Get the axrApplication graphics system
    /// @param app The AxrApplication to use
    /// @returns A handle to the graphics system
    AXR_API AxrGraphicsSystem_T axrApplicationGetGraphicsSystem(AxrApplication_T app);
    /// Get the axrApplication global asset collection
    /// @param app The AxrApplication to use
    /// @returns A handle to the global asset collection
    AXR_API AxrAssetCollection_T axrApplicationGetGlobalAssetCollection(AxrApplication_T app);
}
