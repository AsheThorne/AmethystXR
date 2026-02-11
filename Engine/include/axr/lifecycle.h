#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/vulkanApi.h"

#include <cstdint>

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Application config
struct AxrApplicationConfig {
    uint32_t ApplicationVersion;
    char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE];
};

/// Window config
struct AxrWindowConfig {
    uint32_t Width;
    uint32_t Height;
    char Title[AXR_MAX_WINDOW_TITLE_SIZE];
    bool Enabled;
};

/// Renderer config
struct AxrRendererConfig {
    union {
        AxrVulkanRendererConfig VulkanConfig;
    };
    AxrRendererApiTypeEnum ApiType;
};

/// Engine Config
struct AxrEngineConfig {
    AxrApplicationConfig ApplicationConfig;
    AxrWindowConfig WindowConfig;
    AxrRendererConfig RendererConfig;
};

// ----------------------------------------- //
// External Function Declarations
// ----------------------------------------- //
extern "C" {
    /// Set up the AmethystXR engine
    /// @param config Handle to the engine config
    /// @return AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrSetup(const AxrEngineConfig* config);
    /// Shut down the AmethystXR engine
    AXR_API void axrShutdown();
}
