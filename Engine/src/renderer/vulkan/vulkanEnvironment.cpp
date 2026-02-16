// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanEnvironment.h"

#include "../../memory/allocator.h"
#include "../../platform/platform.h"
#include "vulkanUtils.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set up desktop context. "
AxrResult AxrVulkanEnvironment::setupDesktopContext(const SetupConfig& config, DesktopContext& context) {
    context.Instance = config.Instance;

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = AxrPlatform::get().createVulkanSurface(config.Instance, context.Surface);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create surface.");
        return axrResult;
    }

    axrResult = setDesktopSwapchainFormats(context.Surface,
                                           config.PhysicalDevice,
                                           config.SwapchainColorFormatOptions,
                                           config.SwapchainDepthFormatOptions,
                                           context.SwapchainColorFormat,
                                           context.SwapchainColorSpace,
                                           context.SwapchainDepthFormat);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set desktop swapchain formats.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyDesktopContext(DesktopContext& context) {
    AxrPlatform::get().destroyVulkanSurface(context.Instance, context.Surface);
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set desktop swapchain formats. "
AxrResult AxrVulkanEnvironment::setDesktopSwapchainFormats(const VkSurfaceKHR& surface,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                           const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                           VkFormat& colorFormat,
                                                           VkColorSpaceKHR& colorSpace,
                                                           VkFormat& depthFormat) {
    AxrVector_Stack<VkSurfaceFormatKHR> supportedSurfaceFormats;
    AxrResult axrResult = getSupportedSurfaceFormats(surface, physicalDevice, supportedSurfaceFormats);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats.");
        return axrResult;
    }

    AxrVector_Stack<VkFormat> supportedFormats(supportedSurfaceFormats.size() + swapchainDepthFormatOptions.size(),
                                               &AxrAllocator::get().FrameAllocator);
    for (const VkSurfaceFormatKHR& surfaceFormat : supportedSurfaceFormats) {
        supportedFormats.pushBack(surfaceFormat.format);
    }

    // There's no such thing as 'supported depth formats' for the desktop the same way there are supported color
    // formats. So we just add all depth formats.
    for (VkFormat format : swapchainDepthFormatOptions) {
        supportedFormats.pushBack(format);
    }

    axrResult = setSwapchainFormats(physicalDevice,
                                    swapchainColorFormatOptions,
                                    swapchainDepthFormatOptions,
                                    supportedFormats,
                                    colorFormat,
                                    depthFormat);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats.");
        return axrResult;
    }

    // Set the output color space to the same as the one that the chosen supported color format uses
    for (const VkSurfaceFormatKHR& supportedSurfaceFormat : supportedSurfaceFormats) {
        if (supportedSurfaceFormat.format == colorFormat) {
            colorSpace = supportedSurfaceFormat.colorSpace;
            break;
        }
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats. "
AxrResult AxrVulkanEnvironment::getSupportedSurfaceFormats(const VkSurfaceKHR& surface,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           AxrVector_Stack<VkSurfaceFormatKHR>& supportedFormats) {
    if (surface == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Surface is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t surfaceFormatsCount;
    VkResult vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr);
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    AxrVector_Stack<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount, &AxrAllocator::get().FrameAllocator);
    surfaceFormats.prefillData();
    vkResult =
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, surfaceFormats.data());
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    supportedFormats = std::move(surfaceFormats);
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to set swapchain formats. "
AxrResult AxrVulkanEnvironment::setSwapchainFormats(const VkPhysicalDevice& physicalDevice,
                                                    const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                    const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                    const AxrVector_Stack<VkFormat>& supportedSwapchainFormats,
                                                    VkFormat& colorFormat,
                                                    VkFormat& depthFormat) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    // ---- Find the first color format that is supported ----
    VkFormat selectedColorFormat = VK_FORMAT_UNDEFINED;

    // From our list of options, find the first supported color format that supports our desired features
    for (const VkFormat formatOption : swapchainColorFormatOptions) {
        if (!areFormatFeaturesSupported(formatOption,
                                        VK_IMAGE_TILING_OPTIMAL,
                                        VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
                                            VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT,
                                        physicalDevice)) {
            continue;
        }

        const auto foundFormatIterator = supportedSwapchainFormats.findFirst(formatOption);
        if (foundFormatIterator != supportedSwapchainFormats.end()) {
            selectedColorFormat = *foundFormatIterator;
            break;
        }
    }

    if (selectedColorFormat == VK_FORMAT_UNDEFINED) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a suitable color format to use.");
        return AXR_ERROR_NOT_FOUND;
    }

    // ---- Find the first depth format that is supported ----
    VkFormat selectedDepthFormat = VK_FORMAT_UNDEFINED;

    for (const VkFormat formatOption : swapchainDepthFormatOptions) {
        if (!areFormatFeaturesSupported(formatOption,
                                        VK_IMAGE_TILING_OPTIMAL,
                                        VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT |
                                            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                        physicalDevice)) {
            continue;
        }

        const auto foundFormatIterator = supportedSwapchainFormats.findFirst(formatOption);
        if (foundFormatIterator != supportedSwapchainFormats.end()) {
            selectedDepthFormat = *foundFormatIterator;
            break;
        }
    }

    if (selectedDepthFormat == VK_FORMAT_UNDEFINED) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a suitable depth format to use.");
        return AXR_ERROR_NOT_FOUND;
    }

    colorFormat = selectedColorFormat;
    depthFormat = selectedDepthFormat;

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to check supported format features. "
bool AxrVulkanEnvironment::areFormatFeaturesSupported(const VkFormat format,
                                                      const VkImageTiling imageTiling,
                                                      const VkFormatFeatureFlags features,
                                                      const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return false;
    }

    VkFormatProperties properties{};
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

    if (imageTiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
        return true;
    }
    if (imageTiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
        return true;
    }

    return false;
}
#undef AXR_FUNCTION_FAILED_STRING
