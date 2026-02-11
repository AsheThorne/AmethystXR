#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"

#include <vulkan/vulkan.h>

/// Create a debug messenger object
/// @param instance The instance the messenger will be used with
/// @param pCreateInfo A pointer to a VkDebugUtilsMessengerCreateInfoEXT structure
/// @param pAllocator Controls host memory allocation
/// @param pMessenger A pointer to a VkDebugUtilsMessengerEXT handle in which the created object is returned
/// @returns VK_SUCCESS If the operation was successful
/// @details https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateDebugUtilsMessengerEXT.html
VkResult vkCreateDebugUtilsMessengerEXT(const VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pMessenger) {
    const auto function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (function == nullptr) {
        axrLogError("Failed to get xrCreateDebugUtilsMessengerEXT proc address.");
        return VK_ERROR_UNKNOWN;
    }

    return function(instance, pCreateInfo, pAllocator, pMessenger);
}

/// Destroy a debug messenger object
/// @param instance The instance where the callback was created
/// @param messenger The VkDebugUtilsMessengerEXT object to destroy
/// @param pAllocator Controls host memory allocation
/// @details https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkDestroyDebugUtilsMessengerEXT.html
void vkDestroyDebugUtilsMessengerEXT(const VkInstance instance,
                                     VkDebugUtilsMessengerEXT messenger,
                                     const VkAllocationCallbacks* pAllocator) {
    const auto function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

    if (function == nullptr) {
        axrLogError("Failed to get vkDestroyDebugUtilsMessengerEXT proc address.");
        return;
    }

    return function(instance, messenger, pAllocator);
}

#endif
