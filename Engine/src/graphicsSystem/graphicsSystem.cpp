// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "graphicsSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

void axrGraphicsSystemDrawFrame(const AxrGraphicsSystem_T graphicsSystem) {
    if (graphicsSystem == nullptr) {
        axrLogErrorLocation("`graphicsSystem` is null.");
        return;
    }

    graphicsSystem->drawFrame();
}

void axrGraphicsSystemSetClearColor(const AxrGraphicsSystemConst_T graphicsSystem, const AxrColor color) {
    if (graphicsSystem == nullptr) {
        axrLogErrorLocation("`graphicsSystem` is null.");
        return;
    }

    graphicsSystem->setClearColor(color);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrGraphicsSystem::AxrGraphicsSystem(const Config& config):
    m_GraphicsApi(config.GraphicsConfig.GraphicsApi) {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    if (config.GraphicsConfig.GraphicsApi == AXR_GRAPHICS_API_VULKAN) {
        if (config.GraphicsConfig.VulkanConfig != nullptr) {
            AxrVulkanGraphicsSystem::WindowConfig windowConfig{};
            AxrVulkanGraphicsSystem::XrSessionConfig xrSessionConfig{};
            bool useWindowConfig = false;
            bool useXrSessionConfig = false;

            if (config.WindowSystem != nullptr) {
                if (config.GraphicsConfig.VulkanConfig->WindowConfig != nullptr &&
                    config.GraphicsConfig.GraphicsWindowConfig != nullptr) {
                    windowConfig = AxrVulkanGraphicsSystem::WindowConfig{
                        .PresentationMode = config.GraphicsConfig.VulkanConfig->WindowConfig->PresentationMode,
                        .MaxMsaaSampleCount = config.GraphicsConfig.GraphicsWindowConfig->MaxMsaaSampleCount,
                    };
                    useWindowConfig = true;
                } else {
                    axrLogErrorLocation("Failed to set window graphics config. Missing window graphic configs.");
                }
            }

            if (config.XrSystem != nullptr) {
                if (config.GraphicsConfig.GraphicsXrSessionConfig != nullptr) {
                    xrSessionConfig = AxrVulkanGraphicsSystem::XrSessionConfig{
                        .MaxMsaaSampleCount = config.GraphicsConfig.GraphicsXrSessionConfig->MaxMsaaSampleCount,
                    };
                    useXrSessionConfig = true;
                } else {
                    axrLogErrorLocation(
                        "Failed to set xr session graphics config. Missing xr session graphic configs."
                    );
                }
            }

            m_VulkanGraphicsSystem = new AxrVulkanGraphicsSystem(
                AxrVulkanGraphicsSystem::Config{
                    .ApplicationName = config.ApplicationName,
                    .ApplicationVersion = config.ApplicationVersion,
                    .WindowSystem = config.WindowSystem,
                    .XrSystem = config.XrSystem,
                    .GlobalAssetCollection = config.GlobalAssetCollection,
                    .WindowConfig = useWindowConfig ? &windowConfig : nullptr,
                    .XrSessionConfig = useXrSessionConfig ? &xrSessionConfig : nullptr,
                    .SamplerAnisotropyQuality = config.GraphicsConfig.SamplerAnisotropyQuality,
                    .ApiLayerCount = config.GraphicsConfig.VulkanConfig->ApiLayerCount,
                    .ApiLayers = config.GraphicsConfig.VulkanConfig->ApiLayers,
                    .ExtensionCount = config.GraphicsConfig.VulkanConfig->ExtensionCount,
                    .Extensions = config.GraphicsConfig.VulkanConfig->Extensions,
                }
            );
        } else {
            axrLogErrorLocation("Vulkan config is null.");
        }
    }
#endif
}

AxrGraphicsSystem::~AxrGraphicsSystem() {
    resetSetup();
}

// ---- Public Headers ----

void AxrGraphicsSystem::drawFrame() {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            if (m_VulkanGraphicsSystem == nullptr) {
                axrLogErrorLocation("VulkanGraphicsSystem is null.");
                return;
            }

            m_VulkanGraphicsSystem->drawFrame();
            return;
#elif
            axrLogErrorLocation("Vulkan not supported.");
            return;
#endif
        }
        case AXR_GRAPHICS_API_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return;
        }
    }
}

void AxrGraphicsSystem::setClearColor(const AxrColor& color) const {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            if (m_VulkanGraphicsSystem == nullptr) {
                axrLogErrorLocation("VulkanGraphicsSystem is null.");
                return;
            }

            m_VulkanGraphicsSystem->setClearColor(color);
            return;
#elif
            axrLogErrorLocation("Vulkan not supported.");
            return;
#endif
        }
        case AXR_GRAPHICS_API_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return;
        }
    }
}

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

void AxrGraphicsSystem::resetSetup() {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    if (m_VulkanGraphicsSystem != nullptr) {
        delete m_VulkanGraphicsSystem;
        m_VulkanGraphicsSystem = nullptr;
    }
#endif

    m_GraphicsApi = AXR_GRAPHICS_API_UNDEFINED;
}

AxrResult AxrGraphicsSystem::loadScene(const AxrScene_T scene) {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            if (m_VulkanGraphicsSystem == nullptr) {
                axrLogErrorLocation("VulkanGraphicsSystem is null.");
                return AXR_ERROR;
            }

            return m_VulkanGraphicsSystem->loadScene(scene);
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

AxrResult AxrGraphicsSystem::setActiveScene(const std::string& sceneName) {
    switch (m_GraphicsApi) {
        case AXR_GRAPHICS_API_VULKAN: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            if (m_VulkanGraphicsSystem == nullptr) {
                axrLogErrorLocation("VulkanGraphicsSystem is null.");
                return AXR_ERROR;
            }

            return m_VulkanGraphicsSystem->setActiveScene(sceneName);
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
