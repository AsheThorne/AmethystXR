#pragma once

// Vulkan headers are required for <openxr/openxr_platform.h>
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ----------------------------------------- //
// Vulkan Functions
// ----------------------------------------- //
#include "vulkan/vulkan_core.h"
#endif

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

/// Creates a debug messenger
/// @param instance The instance the messenger will be used with
/// @param createInfo A pointer to a XrDebugUtilsMessengerCreateInfoEXT structure
/// @param messenger A pointer to which the created XrDebugUtilsMessengerEXT object is returned
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrCreateDebugUtilsMessengerEXT(
    XrInstance instance,
    const XrDebugUtilsMessengerCreateInfoEXT* createInfo,
    XrDebugUtilsMessengerEXT* messenger
);

/// Destroys a debug messenger
/// @param instance The instance the messenger was used with
/// @param messenger The XrDebugUtilsMessengerEXT object to destroy
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrDestroyDebugUtilsMessengerEXT(
    XrInstance instance,
    XrDebugUtilsMessengerEXT messenger
);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ---- Vulkan Functions ----

/// Retrieve the Vulkan version requirements for an instance and system
/// @param instance XrInstance handle previously created with xrCreateInstance
/// @param systemId XrSystemId handle for the system which will be used to create a session
/// @param graphicsRequirements The XrGraphicsRequirementsVulkanKHR output structure
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrGetVulkanGraphicsRequirements2KHR(
    XrInstance instance,
    XrSystemId systemId,
    XrGraphicsRequirementsVulkan2KHR* graphicsRequirements
);

/// Create an OpenXR compatible VkInstance
/// @param instance An XrInstance handle previously created with xrCreateInstance
/// @param createInfo Extensible input struct of type XrVulkanInstanceCreateInfoKHR
/// @param vulkanInstance A VkInstance handle to populate with the new Vulkan instance
/// @param vulkanResult A VkResult to populate with the result of the vkCreateInstance operation as returned by XrVulkanInstanceCreateInfoKHR::pfnGetInstanceProcAddr
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrCreateVulkanInstanceKHR(
    XrInstance instance,
    const XrVulkanInstanceCreateInfoKHR* createInfo,
    VkInstance* vulkanInstance,
    VkResult* vulkanResult
);

/// Retrieve the Vulkan physical device associated with an OpenXR instance and system
/// @param instance XrInstance handle previously created with xrCreateInstance
/// @param getInfo Extensible input struct of type XrVulkanGraphicsDeviceGetInfoKHR
/// @param vulkanPhysicalDevice A pointer to a VkPhysicalDevice value to populate
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrGetVulkanGraphicsDevice2KHR(
    XrInstance instance,
    const XrVulkanGraphicsDeviceGetInfoKHR* getInfo,
    VkPhysicalDevice* vulkanPhysicalDevice
);

/// Create an OpenXR compatible VkDevice
/// @param instance An XrInstance handle previously created with xrCreateInstance
/// @param createInfo extensible input struct of type XrCreateVulkanDeviceCreateInfoKHR
/// @param vulkanDevice A VkDevice handle to populate with the new Vulkan device
/// @param vulkanResult A VkResult to populate with the result of the vkCreateDevice operation as returned by XrVulkanInstanceCreateInfoKHR::pfnGetInstanceProcAddr
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrCreateVulkanDeviceKHR(
    XrInstance instance,
    const XrVulkanDeviceCreateInfoKHR* createInfo,
    VkDevice* vulkanDevice,
    VkResult* vulkanResult
);
#endif
