#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "vulkanApi.h"
#endif

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrGraphicsSystem Handle
typedef class AxrGraphicsSystem* AxrGraphicsSystem_T;

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Graphics api enum
enum AxrGraphicsApiEnum {
    AXR_GRAPHICS_API_UNDEFINED = 0,
    AXR_GRAPHICS_API_VULKAN = 1,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the graphics system
struct AxrGraphicsSystemConfig {
    AxrGraphicsApiEnum GraphicsApi;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    AxrVulkanApiConfig_T VulkanConfig;
#endif
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
}
