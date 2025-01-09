#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"

namespace axr {
    // ----------------------------------------- //
    // Result Definition
    // ----------------------------------------- //

    /// Result enum
    enum Result {
        // ---- Failed----
        Error = AXR_ERROR,

        // ---- Succeeded ----
        Success = AXR_SUCCESS,
    };

    static_assert(
        sizeof(AxrResult) == sizeof(axr::Result),
        "Original type and wrapper have different size!"
    );
}
