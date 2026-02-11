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
    /// Error, a previous function failed unexpectedly
    AXR_ERROR_FALLTHROUGH = -2,
    /// Error, the object already exists
    AXR_ERROR_DUPLICATE = -3,
    /// Error, failed to find the object
    AXR_ERROR_NOT_FOUND = -4,
    /// Error, encountered a nullptr
    AXR_ERROR_NULLPTR = -5,
    /// Error, Ran out of memory
    AXR_ERROR_OUT_OF_MEMORY = -6,
    /// Error, Feature isn't supported
    AXR_ERROR_NOT_SUPPORTED = -7,
    /// Error, Validation failed
    AXR_ERROR_VALIDATION_FAILED = -8,
    /// Error, Encountered an error with vulkan
    AXR_ERROR_VULKAN_ERROR = -9,
};

/// Axr renderer api type
enum AxrRendererApiTypeEnum {
    AXR_RENDERER_API_TYPE_UNDEFINED = 0,
    AXR_RENDERER_API_TYPE_VULKAN = 1,
};
