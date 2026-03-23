#pragma once
#ifdef AXR_VULKAN_SUPPORTED

#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"
#include "axr/vulkanApi.h"

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

// ---- Logging ----

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

// ---- Enum Helper Functions ----

/// Convert an AxrVulkanPresentationModeEnum to a VkPresentModeKHR
/// @param presentationMode Presentation mode to convert
/// @returns The converted presentation mode
[[nodiscard]] inline VkPresentModeKHR axrToVkPresentMode(const AxrVulkanPresentationModeEnum presentationMode) {
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

/// Check if the given format includes a stencil component
/// @param format Format to check
/// @returns True if the format includes a stencil component
[[nodiscard]] inline bool axrFormatHasStencilComponent(const VkFormat format) {
    return format == VK_FORMAT_S8_UINT || format == VK_FORMAT_D16_UNORM_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT || format == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

// ---- Memory ----

/// Find a suitable memory type index from the given type filter and properties
/// @param physicalDevice Physical device to use
/// @param typeFilter Type filter
/// @param properties Memory properties
/// @param memoryTypeIndex Output memory type index
/// @returns AXR_SUCCESS if the function succeeded.
/// AXR_ERROR_NOT_FOUND if no memory type index was found.
[[nodiscard]] AxrResult axrFindMemoryTypeIndex(const VkPhysicalDevice& physicalDevice,
                                                      uint32_t typeFilter,
                                                      VkMemoryPropertyFlags properties,
                                                      uint32_t& memoryTypeIndex);

// ---- Command Buffer ----

/// Begin a single time command
/// @param device Device to use
/// @param commandPool Command pool to use
/// @param commandBuffer Output command buffer to record commands on
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrBeginSingleTimeCommand(const VkDevice& device,
                                                  const VkCommandPool& commandPool,
                                                  VkCommandBuffer& commandBuffer);
/// End a single time command
/// @param device Device to use
/// @param commandPool Command pool to use
/// @param queue Queue to use
/// @param commandBuffer Command buffer to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrEndSingleTimeCommand(const VkDevice& device,
                                                const VkCommandPool& commandPool,
                                                const VkQueue& queue,
                                                VkCommandBuffer& commandBuffer);

#endif
