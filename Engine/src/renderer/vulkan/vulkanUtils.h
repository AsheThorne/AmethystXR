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

#endif
