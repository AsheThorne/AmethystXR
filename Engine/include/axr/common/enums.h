#pragma once

// ----------------------------------------- //
// Enums
// ----------------------------------------- //
enum AxrResult {
    // ---- Succeeded ----
    /// The function succeeded as expected
    AXR_SUCCESS = 0,

    // ---- Failed ----
    /// Error, a previous function failed unexpectedly
    AXR_ERROR_FALLTHROUGH = -1,
    /// Error, the object already exists
    AXR_ERROR_DUPLICATE = -2,
    /// Error, failed to find the object
    AXR_ERROR_NOT_FOUND = -3,
    /// Error, encountered a nullptr
    AXR_ERROR_NULLPTR = -4,
};
