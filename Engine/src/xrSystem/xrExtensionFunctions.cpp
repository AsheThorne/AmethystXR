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
