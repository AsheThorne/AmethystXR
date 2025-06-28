// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "xrExtensionFunctions.hpp"
#include "axr/logger.h"

XrResult XRAPI_PTR xrCreateDebugUtilsMessengerEXT(
    const XrInstance instance,
    const XrDebugUtilsMessengerCreateInfoEXT* createInfo,
    XrDebugUtilsMessengerEXT* messenger
) {
    PFN_xrCreateDebugUtilsMessengerEXT function = nullptr;
    const XrResult xrResult = xrGetInstanceProcAddr(
        instance,
        "xrCreateDebugUtilsMessengerEXT",
        reinterpret_cast<PFN_xrVoidFunction*>(&function)
    );

    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation("Failed to get xrCreateDebugUtilsMessengerEXT proc address.");
        return xrResult;
    }

    return function(instance, createInfo, messenger);
}

XrResult XRAPI_PTR xrDestroyDebugUtilsMessengerEXT(
    const XrInstance instance,
    const XrDebugUtilsMessengerEXT messenger
) {
    PFN_xrDestroyDebugUtilsMessengerEXT function = nullptr;
    const XrResult xrResult = xrGetInstanceProcAddr(
        instance,
        "xrDestroyDebugUtilsMessengerEXT",
        reinterpret_cast<PFN_xrVoidFunction*>(&function)
    );

    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation("Failed to get xrDestroyDebugUtilsMessengerEXT proc address.");
        return xrResult;
    }

    return function(messenger);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ---- Vulkan Functions ----

XrResult XRAPI_PTR xrGetVulkanGraphicsRequirements2KHR(
    const XrInstance instance,
    const XrSystemId systemId,
    XrGraphicsRequirementsVulkan2KHR* graphicsRequirements
) {
    PFN_xrGetVulkanGraphicsRequirements2KHR function = nullptr;
    const XrResult xrResult = xrGetInstanceProcAddr(
        instance,
        "xrGetVulkanGraphicsRequirements2KHR",
        reinterpret_cast<PFN_xrVoidFunction*>(&function)
    );

    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation("Failed to get xrGetVulkanGraphicsRequirements2KHR proc address.");
        return xrResult;
    }

    return function(instance, systemId, graphicsRequirements);
}

XrResult XRAPI_PTR xrCreateVulkanInstanceKHR(
    const XrInstance instance,
    const XrVulkanInstanceCreateInfoKHR* createInfo,
    VkInstance* vulkanInstance,
    VkResult* vulkanResult
) {
    PFN_xrCreateVulkanInstanceKHR function = nullptr;
    const XrResult xrResult = xrGetInstanceProcAddr(
        instance,
        "xrCreateVulkanInstanceKHR",
        reinterpret_cast<PFN_xrVoidFunction*>(&function)
    );

    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation("Failed to get xrCreateVulkanInstanceKHR proc address.");
        return xrResult;
    }

    return function(instance, createInfo, vulkanInstance, vulkanResult);
}

XrResult XRAPI_PTR xrGetVulkanGraphicsDevice2KHR(
    const XrInstance instance,
    const XrVulkanGraphicsDeviceGetInfoKHR* getInfo,
    VkPhysicalDevice* vulkanPhysicalDevice
) {
    PFN_xrGetVulkanGraphicsDevice2KHR function = nullptr;
    const XrResult xrResult = xrGetInstanceProcAddr(
        instance,
        "xrGetVulkanGraphicsDevice2KHR",
        reinterpret_cast<PFN_xrVoidFunction*>(&function)
    );

    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation("Failed to get xrGetVulkanGraphicsDevice2KHR proc address.");
        return xrResult;
    }

    return function(instance, getInfo, vulkanPhysicalDevice);
}
#endif
