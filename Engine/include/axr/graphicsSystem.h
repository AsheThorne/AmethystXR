#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/types.h"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "vulkanApi.h"
#endif

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

/// Multisample Anti-Aliasing sample count enum
enum AxrMsaaSampleCountEnum {
    AXR_MSAA_SAMPLE_COUNT_1 = 1,
    AXR_MSAA_SAMPLE_COUNT_2 = 2,
    AXR_MSAA_SAMPLE_COUNT_4 = 4,
    AXR_MSAA_SAMPLE_COUNT_8 = 8,
    AXR_MSAA_SAMPLE_COUNT_16 = 16,
    AXR_MSAA_SAMPLE_COUNT_32 = 32,
    AXR_MSAA_SAMPLE_COUNT_64 = 64,
};

/// Window render source enum
enum AxrWindowRenderSourceEnum {
    AXR_WINDOW_RENDER_SOURCE_SCENE_MAIN_CAMERA = 0,
    AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_LEFT_EYE,
    AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_RIGHT_EYE,
    AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_BOTH_EYES,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the window graphics
struct AxrGraphicsWindowConfig {
    AxrMsaaSampleCountEnum MaxMsaaSampleCount;
};

/// Config for the xr session graphics
struct AxrGraphicsXrSessionConfig {
    AxrMsaaSampleCountEnum MaxMsaaSampleCount;
};

/// Config for the graphics system
struct AxrGraphicsSystemConfig {
    AxrGraphicsApiEnum GraphicsApi;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    AxrVulkanApiConfig_T VulkanConfig;
#endif
    const AxrGraphicsWindowConfig* GraphicsWindowConfig;
    const AxrGraphicsXrSessionConfig* GraphicsXrSessionConfig;
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
    /// Set the clear color
    /// @param graphicsSystem Graphics system to use
    /// @param color Clear color
    AXR_API void axrGraphicsSystemSetClearColor(AxrGraphicsSystemConst_T graphicsSystem, AxrColor color);
    /// Set the window render source
    /// @param graphicsSystem Graphics system to use
    /// @param renderSource Window render source
    AXR_API void axrGraphicsSystemSetWindowRenderSource(
        AxrGraphicsSystemConst_T graphicsSystem,
        AxrWindowRenderSourceEnum renderSource
    );
}
