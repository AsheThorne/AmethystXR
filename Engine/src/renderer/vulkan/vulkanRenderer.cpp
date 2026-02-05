#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanRenderer.h"
#include "axr/logging.h"

#include <cassert>

#include "../../platform/platform.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr vulkan renderer. "
AxrResult AxrVulkanRenderer::setup(Context& context, const Config& config) {
    assert(!context.IsSetup);

    if (config.VulkanConfig == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config.VulkanConfig` is null.");
        return AXR_ERROR_NULLPTR;
    }

    populateApiLayers(config.VulkanConfig->ApiLayerCount, config.VulkanConfig->ApiLayers, context.ApiLayers);
    populateExtensions(config.VulkanConfig->ExtensionCount, config.VulkanConfig->Extensions, context.Extensions);

    context.IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::shutDown(Context& context) {
    context.ApiLayers.clear();
    context.Extensions.clear();
    context.IsSetup = false;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

void AxrVulkanRenderer::populateApiLayers(
    const uint32_t apiLayerCount,
    const AxrVulkanApiLayer apiLayers[],
    AxrExtensionArray<AxrVulkanApiLayer, AxrVulkanApiLayerMaxCount>& dstApiLayers) {
    for (uint32_t i = 0; i < apiLayerCount; ++i) {
        dstApiLayers.pushBack(apiLayers[i]);
    }
}

void AxrVulkanRenderer::populateExtensions(
    const uint32_t extensionCount,
    const AxrVulkanExtension extensions[],
    AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount>& dstExtensions) {
    for (uint32_t i = 0; i < extensionCount; ++i) {
        dstExtensions.pushBack(extensions[i]);
    }

    const AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount> requiredPlatformExtensions =
        AxrPlatform::getRequiredVulkanExtensions();

    for (const AxrVulkanExtension& extension : requiredPlatformExtensions) {
        if (!dstExtensions.exists(extension.Type)) {
            dstExtensions.pushBack(extension);
        }
    }
}

#endif
