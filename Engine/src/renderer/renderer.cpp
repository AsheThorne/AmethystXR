// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "renderer.h"
#include "../memory/allocator.h"
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
    assert(!m_IsSetup);
    assert(config.RendererConfig != nullptr);

    m_Context = AxrRendererContext(config.RendererConfig->ApiType);

    const AxrResult axrResult = axrRendererContextExecute(m_Context, setup_vulkan, config);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        return axrResult;
    }

    m_IsSetup = true;

    return axrResult;
}

void AxrRenderer::shutDown() {
    axrRendererContextExecute(m_Context, shutDown_vulkan);

    m_IsSetup = false;
}

void AxrRenderer::destroyDesktopResources() {
    axrRendererContextExecute(m_Context, destroyDesktopResources_vulkan);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to render active scene. "
// TODO: Pass something called a "Scene Stack" to this function. It will define the scenes we want to render and how
//  they combine into a single final image. For example, we could render a 3D scene and a UI scene, and then combine
//  them before presenting the final image to the screen.
AxrResult AxrRenderer::renderScene() {
    AxrRenderSurface renderSurface;
    AxrResult axrResult = axrRendererContextExecute(m_Context,
                                                    renderScene_vulkan,
                                                    constructRenderCommands(),
                                                    renderSurface);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to render scene.");
        return axrResult;
    }

    axrResult = axrRendererContextExecute(m_Context, presentToDesktop_vulkan, renderSurface);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to present image to desktop environment.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

AxrVector_Stack<AxrRenderCommand> AxrRenderer::constructRenderCommands() {
    AxrVector_Stack<AxrRenderCommand> renderCommands(m_MaxRenderCommandCount, &AxrAllocator::get().FrameAllocator);

    renderCommands.append({
        AxrRenderCommand{
            .RenderData =
                AxrRenderCommandData{
                    .WaitForPreviousFrame = AxrWaitForPreviousFrameCommandData{},
                },
            .Type = AXR_RENDER_COMMAND_TYPE_WAIT_FOR_PREVIOUS_FRAME,
        },
        AxrRenderCommand{
            .RenderData =
                AxrRenderCommandData{
                    .BeginRendering = AxrBeginRenderingCommandData{},
                },
            .Type = AXR_RENDER_COMMAND_TYPE_BEGIN_RENDERING,
        },
        AxrRenderCommand{
            .RenderData =
                AxrRenderCommandData{
                    .EndRendering = AxrEndRenderingCommandData{},
                },
            .Type = AXR_RENDER_COMMAND_TYPE_END_RENDERING,
        },
    });

    return renderCommands;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up vulkan renderer. "
AxrResult AxrRenderer::setup_vulkan(AxrVulkanRendererContext& context, const Config& config) {
#ifdef AXR_VULKAN_SUPPORTED
    // Initialize default variables since it's inside a 'union'
    context = AxrVulkanRendererContext{};

    AxrVulkanRenderer::Config vulkanConfig{
        .VulkanConfig = &config.RendererConfig->VulkanConfig,
        .ApplicationVersion = config.ApplicationVersion,
        .ApplicationName = "",
    };

    std::strncpy(vulkanConfig.ApplicationName, config.ApplicationName, AXR_MAX_APPLICATION_NAME_SIZE);

    return AxrVulkanRenderer::setup(context, vulkanConfig);
#else
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
    return AXR_ERROR_NOT_SUPPORTED;
#endif
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to shut down vulkan renderer. "
void AxrRenderer::shutDown_vulkan(AxrVulkanRendererContext& context) {
#ifdef AXR_VULKAN_SUPPORTED
    AxrVulkanRenderer::shutDown(context);
#else
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
#endif
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to destroy vulkan desktop resources. "
void AxrRenderer::destroyDesktopResources_vulkan(AxrVulkanRendererContext& context) {
#ifdef AXR_VULKAN_SUPPORTED
    AxrVulkanRenderer::destroyDesktopResources(context);
#else
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
#endif
}
#undef AXR_FUNCTION_FAILED_STRING

AxrResult AxrRenderer::renderScene_vulkan(const AxrVulkanRendererContext& context,
                                          const AxrVector_Stack<AxrRenderCommand>& renderCommands,
                                          AxrRenderSurface& renderSurface) {
#ifdef AXR_VULKAN_SUPPORTED
    return AxrVulkanRenderer::renderScene(context, renderCommands, renderSurface.Vulkan);
#else
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
    return AXR_ERROR_NOT_SUPPORTED;
#endif
}

AxrResult AxrRenderer::presentToDesktop_vulkan(const AxrVulkanRendererContext& context,
                                               AxrRenderSurface& renderSurface) {
#ifdef AXR_VULKAN_SUPPORTED
    return AxrVulkanRenderer::presentToDesktop(context, renderSurface.Vulkan);
#else
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
    return AXR_ERROR_NOT_SUPPORTED;
#endif
}
