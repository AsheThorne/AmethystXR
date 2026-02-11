#pragma once

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Axr result
enum AxrResult {
    // ---- Succeeded ----
    /// The function succeeded as expected
    AXR_SUCCESS = 0,

    // ---- Failed ----
    /// Error, an unknown error occurred
    AXR_ERROR_UNKNOWN = -1,
    /// Error, the object already exists
    AXR_ERROR_DUPLICATE = -2,
    /// Error, failed to find the object
    AXR_ERROR_NOT_FOUND = -3,
    /// Error, encountered a nullptr
    AXR_ERROR_NULLPTR = -4,
    /// Error, Ran out of memory
    AXR_ERROR_OUT_OF_MEMORY = -5,
    /// Error, Feature isn't supported
    AXR_ERROR_NOT_SUPPORTED = -6,
    /// Error, Validation failed
    AXR_ERROR_VALIDATION_FAILED = -7,
    /// Error, Encountered an error with vulkan
    AXR_ERROR_VULKAN_ERROR = -8,
};

/// Axr renderer api type
enum AxrRendererApiTypeEnum {
    AXR_RENDERER_API_TYPE_UNDEFINED = 0,
    AXR_RENDERER_API_TYPE_VULKAN = 1,
};
