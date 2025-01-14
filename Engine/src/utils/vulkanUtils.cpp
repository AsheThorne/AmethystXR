#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "vulkanUtils.hpp"

bool axrVkSucceeded(vk::Result result) {
    return VK_SUCCEEDED(static_cast<VkResult>(result));
}

bool axrVkFailed(vk::Result result) {
    return VK_FAILED(static_cast<VkResult>(result));
}

#endif
