// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "graphicsSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrGraphicsSystem::AxrGraphicsSystem(const Config& config):
    m_GraphicsApi(config.GraphicsConfig.GraphicsApi) {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    if (config.GraphicsConfig.GraphicsApi == AXR_GRAPHICS_API_VULKAN) {
        m_VulkanGraphicsSystem = std::make_unique<AxrVulkanGraphicsSystem>(
            AxrVulkanGraphicsSystem::Config{
                .ApplicationName = config.ApplicationName,
                .ApplicationVersion = config.ApplicationVersion,
                .WindowPlatform = config.WindowPlatform,
                .WindowSystem = config.WindowSystem,
                .VulkanConfig = config.GraphicsConfig.VulkanConfig
            }
        );
    }
#endif
}

// ---- Public Headers ----

AxrResult AxrGraphicsSystem::setup() {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
            return setupVulkan();
        }
        case AXR_GRAPHICS_API_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return AXR_ERROR;
        }
    }
}

// ---- Private Functions ----

AxrResult AxrGraphicsSystem::setupVulkan() {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    if (m_VulkanGraphicsSystem == nullptr) {
        axrLogErrorLocation("VulkanGraphicsSystem is null.");
        return AXR_ERROR;
    }

    return m_VulkanGraphicsSystem->setup();
#elif
    axrLogErrorLocation(
           "Vulkan not supported."
       );
    return AXR_ERROR;
#endif
}
