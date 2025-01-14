#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

/// Check if the vulkan result succeeded
/// @param result Vulkan result
/// @returns True if the result succeeded
bool axrVkSucceeded(vk::Result result);

/// Check if the vulkan result failed
/// @param result Vulkan result
/// @returns True if the result failed
bool axrVkFailed(vk::Result result);

#endif
