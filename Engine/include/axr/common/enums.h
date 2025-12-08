#pragma once

// ----------------------------------------- //
// Enums
// ----------------------------------------- //
enum AxrResult {
    // ---- Succeeded ----
    AXR_SUCCESS = 0,
    AXR_DUPLICATE = 1,

    // ---- Failed ----
    AXR_ERROR_UNKNOWN = -1,
    AXR_ERROR_NOT_FOUND = -2,
};
