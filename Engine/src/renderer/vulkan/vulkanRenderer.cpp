// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanRenderer.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr vulkan renderer. "
AxrResult AxrVulkanRenderer::setup(Context& context, const Config& config) {
#ifndef AXR_VULKAN_SUPPORTED
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
    return AXR_ERROR_NOT_SUPPORTED;
#else
    assert(!context.IsSetup);

    if (config.VulkanConfig == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config.VulkanConfig` is null.");
        return AXR_ERROR_NULLPTR;
    }

    context.IsSetup = true;
    return AXR_SUCCESS;
#endif
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to shut down axr vulkan renderer. "
void AxrVulkanRenderer::shutDown(Context& context) {
#ifndef AXR_VULKAN_SUPPORTED
    axrLogError(AXR_FUNCTION_FAILED_STRING "Vulkan isn't supported.");
#else
    context.IsSetup = false;
#endif
}
#undef AXR_FUNCTION_FAILED_STRING

#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Private Function Definitions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

#endif
