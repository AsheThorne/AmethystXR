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

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Vec2 as an AxrVec2
        /// @returns This as an AxrVec2
        const AxrVec2* toRaw() const {
            return reinterpret_cast<const AxrVec2*>(this);
        }

        /// Get a handle to the Vec2 as an AxrVec2
        /// @returns This as an AxrVec2
        AxrVec2* toRaw() {
            return reinterpret_cast<AxrVec2*>(this);
        }
    };

    static_assert(
        sizeof(AxrVec2) == sizeof(axr::Vec2),
        "Original type and wrapper have different size!"
    );

    /// Axr Color
    struct Color {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        float r;
        float g;
        float b;
        float a;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        Color(const float r, const float g, const float b, const float a):
            r(r),
            g(g),
            b(b),
            a(a) {
        }

        explicit Color(const AxrColor& color):
            r(color.r),
            g(color.g),
            b(color.b),
            a(color.a) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Color as an AxrColor
        /// @returns This as an AxrColor
        const AxrColor* toRaw() const {
            return reinterpret_cast<const AxrColor*>(this);
        }

        /// Get a handle to the Color as an AxrColor
        /// @returns This as an AxrColor
        AxrColor* toRaw() {
            return reinterpret_cast<AxrColor*>(this);
        }
    };

    static_assert(
        sizeof(AxrColor) == sizeof(axr::Color),
        "Original type and wrapper have different size!"
    );
}
