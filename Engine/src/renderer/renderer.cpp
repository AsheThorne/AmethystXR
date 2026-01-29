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

AxrResult AxrRenderer::setup(const Config& config) {
    auto vulkan = [](AxrVulkanRenderer::Context& context) -> AxrResult {
        return AxrVulkanRenderer::setup(context,
                                        AxrVulkanRenderer::Config{
                                            // TODO...
                                            .ApplicationName = "",
                                            .ApplicationVersion = 0,
                                        });
    };

    return axrRendererContextExecute(m_Context, vulkan);
}

void AxrRenderer::shutDown() {
    auto vulkan = [](AxrVulkanRenderer::Context& context) -> void {
        AxrVulkanRenderer::shutDown(context);
    };

    axrRendererContextExecute(m_Context, vulkan);
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //
