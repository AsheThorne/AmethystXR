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

vk::Format axrChooseFormat(
    const std::vector<vk::Format>& formatOptions,
    const std::vector<vk::Format>& availableFormats
) {
    const auto fallbackFormatIt = std::find_first_of(
        formatOptions.begin(),
        formatOptions.end(),
        availableFormats.begin(),
        availableFormats.end()
    );

    if (fallbackFormatIt != formatOptions.end()) {
        return *fallbackFormatIt;
    }

    axrLogErrorLocation("Failed to find an available format to use.");
    return vk::Format::eUndefined;
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

#endif
