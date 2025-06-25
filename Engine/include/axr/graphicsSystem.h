#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "vulkanApi.h"
#endif

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Sampler anisotropy quality enum
enum AxrSamplerAnisotropyQualityEnum {
    AXR_SAMPLER_ANISOTROPY_QUALITY_NONE = 0,
    AXR_SAMPLER_ANISOTROPY_QUALITY_LOW,
    AXR_SAMPLER_ANISOTROPY_QUALITY_MEDIUM,
    AXR_SAMPLER_ANISOTROPY_QUALITY_HIGH,
};

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
    glm::vec4 ClearColor;
    AxrSamplerAnisotropyQualityEnum SamplerAnisotropyQuality;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrGraphicsSystem Handle
typedef class AxrGraphicsSystem* AxrGraphicsSystem_T;
/// const AxrGraphicsSystem Handle
typedef const AxrGraphicsSystem* AxrGraphicsSystemConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Draw the current frame
    /// @param graphicsSystem Graphics system to use
    AXR_API void axrGraphicsSystemDrawFrame(AxrGraphicsSystem_T graphicsSystem);
}
