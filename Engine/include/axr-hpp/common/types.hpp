#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/types.h"

namespace axr {
    /// Axr Vec2
    struct Vec2 {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        
        float x;
        float y;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        Vec2(const float x, const float y):
            x(x),
            y(y) {
        }

        explicit Vec2(const AxrVec2& vec2):
            x(vec2.x),
            y(vec2.y) {
        }
    };

    static_assert(
        sizeof(AxrVec2) == sizeof(axr::Vec2),
        "Original type and wrapper have different size!"
    );
}
