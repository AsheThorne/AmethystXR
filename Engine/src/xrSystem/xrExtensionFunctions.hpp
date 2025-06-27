#pragma once

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr.h>

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