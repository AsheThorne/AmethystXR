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
        m_VulkanGraphicsSystem = new AxrVulkanGraphicsSystem(
            AxrVulkanGraphicsSystem::Config{
                .ApplicationName = config.ApplicationName,
                .ApplicationVersion = config.ApplicationVersion,
                .WindowSystem = config.WindowSystem,
                .VulkanConfig = config.GraphicsConfig.VulkanConfig
            }
        );
    }
#endif
}

AxrGraphicsSystem::~AxrGraphicsSystem() {
    cleanup();
}

// ---- Public Headers ----

AxrResult AxrGraphicsSystem::setup() {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            if (m_VulkanGraphicsSystem == nullptr) {
                axrLogErrorLocation("VulkanGraphicsSystem is null.");
                return AXR_ERROR;
            }

            return m_VulkanGraphicsSystem->setup();
#elif
            axrLogErrorLocation("Vulkan not supported.");
            return AXR_ERROR;
#endif
        }
        case AXR_GRAPHICS_API_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return AXR_ERROR;
        }
    }
}

void AxrGraphicsSystem::cleanup() {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    if (m_VulkanGraphicsSystem != nullptr) {
        delete m_VulkanGraphicsSystem;
        m_VulkanGraphicsSystem = nullptr;
    }
#endif

    m_GraphicsApi = AXR_GRAPHICS_API_UNDEFINED;
}
