#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include <compare>

namespace axr {
    // ----------------------------------------- //
    // Result
    // ----------------------------------------- //

    /// Result enum
    enum class Result {
        // ---- Failed----
        Error = AXR_ERROR,

        // ---- Succeeded ----
        Success = AXR_SUCCESS,
    };

    inline std::strong_ordering operator<=>(const axr::Result& result, const int number) noexcept {
        return static_cast<AxrResult>(result) <=> number;
    }

    // ----------------------------------------- //
    // Platform Type
    // ----------------------------------------- //

    /// Axr Platform Type
    enum class AxrPlatformType {
        Undefined = AXR_PLATFORM_TYPE_UNDEFINED,
        Window = AXR_PLATFORM_TYPE_WINDOW,
        XrDevice = AXR_PLATFORM_TYPE_XR_DEVICE,
    };
}
