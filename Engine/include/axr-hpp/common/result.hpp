#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"
#include <compare>

namespace axr {
    // ----------------------------------------- //
    // Result Definition
    // ----------------------------------------- //

    /// Result enum
    enum class Result {
        // ---- Failed----
        Error = AXR_ERROR,

        // ---- Succeeded ----
        Success = AXR_SUCCESS,
    };

    static_assert(
        sizeof(AxrResult) == sizeof(axr::Result),
        "Original type and wrapper have different size!"
    );

    inline std::strong_ordering operator<=>(const axr::Result& result, const int number) noexcept {
        return static_cast<AxrResult>(result) <=> number;
    }
}
