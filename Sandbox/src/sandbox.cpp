// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "sandbox.h"
#include "axr/axr.h"

// ----------------------------------------- //
// Sandbox
// ----------------------------------------- //

int start() {
    axrLoggerSetup("Sandbox");

#ifndef AXR_VULKAN_SUPPORTED
    constexpr AxrVulkanRendererConfig vulkanConfig{};
#else
    constexpr AxrVulkanRendererConfig vulkanConfig{
        .ApiLayers =
            {
#ifndef NDEBUG
                AxrVulkanApiLayer{
                    .CoreValidation = AxrVulkanApiLayerCoreValidation{},
                    .Type = AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION,
                },
#endif
            },
        .Extensions =
            {
#ifndef NDEBUG
                AxrVulkanExtension{
                    .DebugUtils =
                        AxrVulkanExtensionDebugUtils{
                            .SeverityFlags = AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_WARNING_BIT |
                                             AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_ERROR_BIT,
                            .TypeFlags = AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_GENERAL_BIT |
                                         AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_VALIDATION_BIT |
                                         AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_PERFORMANCE_BIT},
                    .Type = AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS,
                    .IsRequired = false,
                },
#endif
            },
#ifndef NDEBUG
        .ApiLayerCount = 1,
        .ExtensionCount = 1,
#else
        .ApiLayerCount = 0,
        .ExtensionCount = 0,
#endif
    };
#endif

    constexpr AxrEngineConfig axrEngineConfig{
        .ApplicationConfig =
            AxrApplicationConfig{
                .ApplicationVersion = AXR_MAKE_VERSION(0, 1, 0),
                .ApplicationName = "Sandbox",
            },
        .WindowConfig =
            AxrWindowConfig{
                .Width = 800,
                .Height = 600,
                .Title = "Sandbox",
                .Enabled = true,
            },
        .RendererConfig =
            AxrRendererConfig{
                .VulkanConfig = vulkanConfig,
                .ApiType = AXR_RENDERER_API_TYPE_VULKAN,
            },
    };
    axrSetup(&axrEngineConfig);

    while (axrApplicationIsRunning()) {
        const AxrResult axrResult = axrApplicationStartNewFrame();
        if (axrResult == AXR_APPLICATION_CLOSED) [[unlikely]] {
            break;
        }
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError("Unknown error occurred during application start new frame. Received result: {}",
                        axrResultToString(axrResult));
            break;
        }
    }

    axrShutdown();
    return 0;
}
