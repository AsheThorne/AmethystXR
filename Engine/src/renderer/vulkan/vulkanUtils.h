#pragma once
#ifdef AXR_VULKAN_SUPPORTED

#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

/// Log the vkResult
/// @param vkResult VkResult enum
/// @param functionName Vulkan function which produced the result
/// @param location Source location
inline void axrLogVkResult(const VkResult vkResult,
                           const char* functionName,
                           const std::source_location& location = std::source_location::current()) {
    if (VK_FAILED(vkResult)) {
        axrLogError({"{0} failed with a result of {1}.", location}, functionName, string_VkResult(vkResult));
    }
}

/// Convert an AxrVulkanPresentationModeEnum to a VkPresentModeKHR
/// @param presentationMode Presentation mode to convert
/// @returns The converted presentation mode
inline VkPresentModeKHR axrToVkPresentMode(const AxrVulkanPresentationModeEnum presentationMode) {
    switch (presentationMode) {
        case AXR_VULKAN_PRESENTATION_MODE_IMMEDIATE: {
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
        case AXR_VULKAN_PRESENTATION_MODE_MAILBOX: {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
        case AXR_VULKAN_PRESENTATION_MODE_FIFO: {
            return VK_PRESENT_MODE_FIFO_KHR;
        }
        case AXR_VULKAN_PRESENTATION_MODE_FIFO_RELAXED: {
            return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        }
        case AXR_VULKAN_PRESENTATION_MODE_UNDEFINED:
        default: {
            return VK_PRESENT_MODE_MAX_ENUM_KHR;
        }
    }
}

#endif
