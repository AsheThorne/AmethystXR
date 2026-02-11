#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/vulkanApi.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrVulkanApiLayerTypeEnumToString(const AxrVulkanApiLayerTypeEnum apiLayerType) {
    switch (apiLayerType) {
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED: {
            return "AXR_VULKAN_API_LAYER_TYPE_UNDEFINED";
        }
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            return "AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION";
        }
        case AXR_VULKAN_API_LAYER_TYPE_END: {
            return "AXR_VULKAN_API_LAYER_TYPE_END";
        }
        default: {
            return "Unknown AxrVulkanApiLayerTypeEnum";
        }
    }
}

const char* axrVulkanExtensionTypeEnumToString(const AxrVulkanExtensionTypeEnum extensionType) {
    switch (extensionType) {
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED: {
            return "AXR_VULKAN_EXTENSION_TYPE_UNDEFINED";
        }
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            return "AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS";
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            return "AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN";
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            return "AXR_VULKAN_EXTENSION_TYPE_SURFACE";
        }
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
            return "AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE";
        }
        case AXR_VULKAN_EXTENSION_TYPE_WAYLAND_SURFACE: {
            return "AXR_VULKAN_EXTENSION_TYPE_WAYLAND_SURFACE";
        }
        case AXR_VULKAN_EXTENSION_TYPE_END: {
            return "AXR_VULKAN_EXTENSION_TYPE_END";
        }
        default: {
            return "Unknown AxrVulkanExtensionTypeEnum";
        }
    }
}

#endif
