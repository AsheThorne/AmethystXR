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
/// @param instance is an XrInstance handle previously created with xrCreateInstance
/// @param createInfo extensible input struct of type XrVulkanInstanceCreateInfoKHR
/// @param vulkanInstance points to a VkInstance handle to populate with the new Vulkan instance
/// @param vulkanResult points to a VkResult to populate with the result of the vkCreateInstance operation as returned by XrVulkanInstanceCreateInfoKHR::pfnGetInstanceProcAddr
/// @returns XR_SUCCESS If the operation was successful
XrResult XRAPI_PTR xrCreateVulkanInstanceKHR(
    XrInstance instance,
    const XrVulkanInstanceCreateInfoKHR* createInfo,
    VkInstance* vulkanInstance,
    VkResult* vulkanResult
);
#endif
