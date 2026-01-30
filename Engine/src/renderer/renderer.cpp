// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "renderer.h"
#include "axr/logging.h"
#include "utils.h"
#include "vulkan/vulkanRenderer.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrRenderer::AxrRenderer() = default;

AxrRenderer::~AxrRenderer() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrRenderer& AxrRenderer::get() {
    static AxrRenderer singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr renderer. "
AxrResult AxrRenderer::setup(const Config& config) {
    assert(!m_IsSetup);

    if (config.RendererConfig == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`rendererConfig.RendererConfig` is null.");
        return AXR_ERROR_NULLPTR;
    }

    m_Context = Context{
        .ApiType = config.RendererConfig->ApiType,
    };

    auto vulkan = [](AxrVulkanRenderer::Context& context, const Config& config) -> AxrResult {
#ifndef AXR_VULKAN_SUPPORTED
        axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
        return AXR_ERROR_NOT_SUPPORTED;
#else
        AxrVulkanRenderer::Config vulkanConfig{
            .VulkanConfig = &config.RendererConfig->VulkanConfig,
            .ApplicationVersion = config.ApplicationVersion,
            .ApplicationName = "",
        };

        std::strncpy(vulkanConfig.ApplicationName, config.ApplicationName, AXR_MAX_APPLICATION_NAME_SIZE);

        return AxrVulkanRenderer::setup(context, vulkanConfig);
#endif
    };

    const AxrResult axrResult = axrRendererContextExecute(m_Context, vulkan, config);
    if (AXR_FAILED(axrResult)) {
        return AXR_ERROR_FALLTHROUGH;
    }

    m_IsSetup = true;

    return axrResult;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrRenderer::shutDown() {
    auto vulkan = [](AxrVulkanRenderer::Context& context) -> void {
        AxrVulkanRenderer::shutDown(context);
    };

    axrRendererContextExecute(m_Context, vulkan);

    m_IsSetup = false;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //
