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
AxrResult AxrRenderer::setup(const Config& rendererConfig) {
    assert(!m_IsSetup);

    m_Context = Context{
        .ApiType = rendererConfig.ApiType,
    };

    auto vulkan = [](AxrVulkanRenderer::Context& context, const Config& config) -> AxrResult {
        AxrVulkanRenderer::Config vulkanConfig{
            .ApplicationVersion = config.ApplicationVersion,
            .ApplicationName = "",
        };

        std::strncpy(vulkanConfig.ApplicationName, config.ApplicationName, AXR_MAX_APPLICATION_NAME_SIZE);

        return AxrVulkanRenderer::setup(context, vulkanConfig);
    };

    const AxrResult axrResult = axrRendererContextExecute(m_Context, vulkan, rendererConfig);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "axrRendererContextExecute() failed.");
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
