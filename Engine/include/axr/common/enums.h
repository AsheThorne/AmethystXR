#pragma once

// ----------------------------------------- //
// Enums
// ----------------------------------------- //
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
};
