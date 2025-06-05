#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>

void axrLogVkResult(
    const vk::Result result,
    const char* functionName,
    const std::source_location& location
) {
    if (VK_FAILED(result)) {
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

AxrResult axrFindMemoryTypeIndex(
    const vk::PhysicalDevice& physicalDevice,
    const uint32_t typeFilter,
    const vk::MemoryPropertyFlags properties,
    uint32_t& memoryTypeIndex,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties(dispatch);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            memoryTypeIndex = i;
            return AXR_SUCCESS;
        }
    }

    axrLogErrorLocation("Failed to find a suitable memory type.");
    return AXR_ERROR;
}

AxrResult axrBeginSingleTimeCommand(
    const vk::Device device,
    const vk::CommandPool commandPool,
    vk::CommandBuffer& commandBuffer,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (commandBuffer != VK_NULL_HANDLE) {
        axrLogErrorLocation("Command buffer already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (commandPool == VK_NULL_HANDLE) {
        axrLogErrorLocation("Command pool is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::CommandBufferAllocateInfo commandBufferAllocateInfo(
        commandPool,
        vk::CommandBufferLevel::ePrimary,
        1
    );

    vk::Result vkResult = device.allocateCommandBuffers(
        &commandBufferAllocateInfo,
        &commandBuffer,
        dispatch
    );
    axrLogVkResult(vkResult, "device.allocateCommandBuffers");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    constexpr vk::CommandBufferBeginInfo commandBufferBeginInfo(
        vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
        nullptr
    );

    vkResult = commandBuffer.begin(&commandBufferBeginInfo, dispatch);
    axrLogVkResult(vkResult, "commandBuffer.begin");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult axrEndSingleTimeCommand(
    const vk::Device device,
    const vk::CommandPool commandPool,
    const vk::Queue queue,
    vk::CommandBuffer& commandBuffer,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    vk::Result vkResult = commandBuffer.end(dispatch);
    axrLogVkResult(vkResult, "commandBuffer.end");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    const vk::SubmitInfo submitInfo(
        0,
        {},
        {},
        1,
        &commandBuffer,
        0,
        {}
    );

    vkResult = queue.submit(1, &submitInfo, VK_NULL_HANDLE, dispatch);
    axrLogVkResult(vkResult, "queue.submit");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    vkResult = queue.waitIdle(dispatch);
    axrLogVkResult(vkResult, "queue.waitIdle");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    device.freeCommandBuffers(commandPool, 1, &commandBuffer, dispatch);
    commandBuffer = VK_NULL_HANDLE;

    return AXR_SUCCESS;
}

vk::Filter axrToVkFilter(const AxrImageSamplerFilterEnum samplerFilter) {
    switch (samplerFilter) {
        case AXR_IMAGE_SAMPLER_FILTER_NEAREST: {
            return vk::Filter::eNearest;
        }
        case AXR_IMAGE_SAMPLER_FILTER_LINEAR: {
            return vk::Filter::eLinear;
        }
        case AXR_IMAGE_SAMPLER_FILTER_UNDEFINED:
        default: {
            axrLogErrorLocation("Unknown vk::Filter type.");
            return static_cast<vk::Filter>(VK_FILTER_MAX_ENUM);
        }
    }
}

vk::SamplerAddressMode axrToVkSamplerAddressMode(const AxrImageSamplerWrappingEnum samplerWrapping) {
    switch (samplerWrapping) {
        case AXR_IMAGE_SAMPLER_WRAPPING_REPEAT: {
            return vk::SamplerAddressMode::eRepeat;
        }
        case AXR_IMAGE_SAMPLER_WRAPPING_MIRRORED_REPEAT: {
            return vk::SamplerAddressMode::eMirroredRepeat;
        }
        case AXR_IMAGE_SAMPLER_WRAPPING_CLAMP_TO_EDGE: {
            return vk::SamplerAddressMode::eClampToEdge;
        }
        case AXR_IMAGE_SAMPLER_WRAPPING_CLAMP_TO_BORDER: {
            return vk::SamplerAddressMode::eClampToBorder;
        }
        case AXR_IMAGE_SAMPLER_WRAPPING_UNDEFINED:
        default: {
            axrLogErrorLocation("Unknown vk::SamplerAddressMode type.");
            return static_cast<vk::SamplerAddressMode>(VK_SAMPLER_ADDRESS_MODE_MAX_ENUM);
        }
    }
}

#endif
