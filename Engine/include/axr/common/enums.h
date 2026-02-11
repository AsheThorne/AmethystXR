#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Axr result
enum AxrResult {
    // ---- Succeeded ----
    /// The function succeeded as expected
    AXR_SUCCESS = 0,
    /// The application has closed. The program should exit
    AXR_APPLICATION_CLOSED = 1,

    // ---- Failed ----
    /// Error, an unknown error occurred
    AXR_ERROR_UNKNOWN = -1,
    /// Error, the object already exists
    AXR_ERROR_DUPLICATE = -2,
    /// Error, failed to find the object
    AXR_ERROR_NOT_FOUND = -3,
    /// Error, Ran out of memory
    AXR_ERROR_OUT_OF_MEMORY = -4,
    /// Error, Feature isn't supported
    AXR_ERROR_NOT_SUPPORTED = -5,
    /// Error, Validation failed
    AXR_ERROR_VALIDATION_FAILED = -6,
    /// Error, Encountered an error with vulkan
    AXR_ERROR_VULKAN_ERROR = -7,
};

/// Axr renderer api type
enum AxrRendererApiTypeEnum {
    AXR_RENDERER_API_TYPE_UNDEFINED = 0,
    AXR_RENDERER_API_TYPE_VULKAN = 1,
};

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

extern "C" {
    /// Get the string of the given axrResult
    /// @param axrResult AxrResult to use
    /// @return Given axrResult as a string
    AXR_API const char* axrResultToString(AxrResult axrResult);
}
