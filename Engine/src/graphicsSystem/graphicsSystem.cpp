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
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_VulkanGraphicsSystem(
        {
            .ApplicationName = config.ApplicationName,
            .ApplicationVersion = config.ApplicationVersion,
        }
    ),
#endif
    m_Config(config) {
}

// ---- Public Headers ----

AxrResult AxrGraphicsSystem::setup() {
    switch (m_Config.GraphicsApi) {
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
    return m_VulkanGraphicsSystem.setup();
#elif
    axrLogErrorLocation(
           "Vulkan not supported."
       );
    return AXR_ERROR;
#endif
}
