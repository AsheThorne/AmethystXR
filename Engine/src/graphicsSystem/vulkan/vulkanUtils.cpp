#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "axr/logger.h"
#include "axr/common/defines.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>

bool axrVkSucceeded(vk::Result result) {
    return VK_SUCCEEDED(static_cast<VkResult>(result));
}

bool axrVkFailed(vk::Result result) {
    return VK_FAILED(static_cast<VkResult>(result));
}

void axrLogVkResult(
    const vk::Result result,
    const char* functionName,
    const std::source_location& location
) {
    if (axrVkFailed(result)) {
        axrLogErrorLocation(
            {"{0} failed with a result of {1}.", location},
            functionName,
            to_string(result).c_str()
        );
    }
}

vk::PresentModeKHR axrToVkPresentMode(const AxrVulkanPresentationModeEnum presentationMode) {
    switch (presentationMode) {
        case AXR_VULKAN_PRESENTATION_MODE_IMMEDIATE: {
            return vk::PresentModeKHR::eImmediate;
        }
        case AXR_VULKAN_PRESENTATION_MODE_MAILBOX: {
            return vk::PresentModeKHR::eMailbox;
        }
        case AXR_VULKAN_PRESENTATION_MODE_FIFO: {
            return vk::PresentModeKHR::eFifo;
        }
        case AXR_VULKAN_PRESENTATION_MODE_FIFO_RELAXED: {
            return vk::PresentModeKHR::eFifoRelaxed;
        }
        case AXR_VULKAN_PRESENTATION_MODE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return static_cast<vk::PresentModeKHR>(VK_PRESENT_MODE_MAX_ENUM_KHR);
        }
    }
}

const char* axrGetApiLayerName(const AxrVulkanApiLayerTypeEnum apiLayerType) {
    switch (apiLayerType) {
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            return "VK_LAYER_KHRONOS_validation";
        }
        case AXR_VULKAN_API_LAYER_TYPE_END:
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(apiLayerType));
            return "";
        }
    }
}

const char* axrGetExtensionName(const AxrVulkanExtensionTypeEnum extensionType) {
    switch (extensionType) {
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            return VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            return VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            return VK_KHR_SURFACE_EXTENSION_NAME;
        }
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
#ifdef AXR_USE_PLATFORM_WIN32
            return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#else
            axrLogErrorLocation("Win32 surface extension not supported.");
            return "";
#endif
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extensionType));
            return "";
        }
    }
}

bool axrAreFormatFeaturesSupported(
    const vk::Format format,
    const vk::ImageTiling tiling,
    const vk::FormatFeatureFlags features,
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return false;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(format, dispatch);
    if (tiling == vk::ImageTiling::eLinear && (formatProperties.linearTilingFeatures & features) == features) {
        return true;
    }
    if (tiling == vk::ImageTiling::eOptimal && (formatProperties.optimalTilingFeatures & features) == features) {
        return true;
    }

    return false;
}

AxrResult axrCreateImageViews(
    const vk::Device& device,
    const std::vector<vk::Image>& images,
    const vk::Format format,
    const vk::ImageAspectFlags aspectMask,
    const uint32_t mipLevels,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!imageViews.empty()) {
        axrLogErrorLocation("Image views already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (images.empty()) {
        axrLogErrorLocation("Images are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;
    imageViews.resize(images.size());

    for (size_t i = 0; i < imageViews.size(); ++i) {
        axrResult = axrCreateImageView(
            device,
            images[i],
            format,
            aspectMask,
            mipLevels,
            imageViews[i],
            dispatch
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrDestroyImageViews(device, imageViews, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyImageViews(
    const vk::Device& device,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
) {
    for (vk::ImageView& imageView : imageViews) {
        axrDestroyImageView(device, imageView, dispatch);
    }
    imageViews.clear();
}

AxrResult axrCreateImageView(
    const vk::Device& device,
    const vk::Image& image,
    const vk::Format format,
    const vk::ImageAspectFlags aspectMask,
    const uint32_t mipLevels,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (imageView != VK_NULL_HANDLE) {
        axrLogErrorLocation("Image view already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (image == VK_NULL_HANDLE) {
        axrLogErrorLocation("Image is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::ImageViewCreateInfo imageViewCreateInfo(
        {},
        image,
        vk::ImageViewType::e2D,
        format,
        vk::ComponentMapping(),
        vk::ImageSubresourceRange{
            aspectMask,
            0,
            mipLevels,
            0,
            1
        }
    );

    const auto imageViewResult = device.createImageView(imageViewCreateInfo, nullptr, dispatch);
    axrLogVkResult(imageViewResult.result, "device.createImageView");
    if (axrVkFailed(imageViewResult.result)) {
        return AXR_ERROR;
    }

    imageView = imageViewResult.value;

    return AXR_SUCCESS;
}

void axrDestroyImageView(
    const vk::Device& device,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (imageView == VK_NULL_HANDLE) return;

    device.destroyImageView(imageView, nullptr, dispatch);
    imageView = VK_NULL_HANDLE;
}

#endif
