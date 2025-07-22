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
        float x = 0.0f;
        float y = 0.0f;

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

    /// Axr Vec3
    struct Vec3 {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        Vec3(const float x, const float y, const float z):
            x(x),
            y(y),
            z(z) {
        }

        explicit Vec3(const AxrVec3& vec3):
            x(vec3.x),
            y(vec3.y),
            z(vec3.z) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Vec3 as an AxrVec3
        /// @returns This as an AxrVec3
        const AxrVec3* toRaw() const {
            return reinterpret_cast<const AxrVec3*>(this);
        }

        /// Get a handle to the Vec3 as an AxrVec3
        /// @returns This as an AxrVec3
        AxrVec3* toRaw() {
            return reinterpret_cast<AxrVec3*>(this);
        }
    };

    static_assert(
        sizeof(AxrVec3) == sizeof(axr::Vec3),
        "Original type and wrapper have different size!"
    );

    /// Axr Color
    struct Color {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;

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

    /// Axr Quaternion
    struct Quaternion {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 1.0f;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        Quaternion(const float x, const float y, const float z, const float w):
            x(x),
            y(y),
            z(z),
            w(w) {
        }

        explicit Quaternion(const AxrQuaternion& quaternion):
            x(quaternion.x),
            y(quaternion.y),
            z(quaternion.z),
            w(quaternion.w) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Quaternion as an AxrQuaternion
        /// @returns This as an AxrQuaternion
        const AxrQuaternion* toRaw() const {
            return reinterpret_cast<const AxrQuaternion*>(this);
        }

        /// Get a handle to the Quaternion as an AxrQuaternion
        /// @returns This as an AxrQuaternion
        AxrQuaternion* toRaw() {
            return reinterpret_cast<AxrQuaternion*>(this);
        }
    };

    static_assert(
        sizeof(AxrQuaternion) == sizeof(axr::Quaternion),
        "Original type and wrapper have different size!"
    );

    /// Axr Pose
    struct Pose {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::Vec3 position;
        axr::Quaternion orientation;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        Pose(const axr::Vec3 position, const axr::Quaternion orientation):
            position(position),
            orientation(orientation) {
        }

        explicit Pose(const AxrPose& pose):
            position(pose.position),
            orientation(pose.orientation) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Pose as an AxrPose
        /// @returns This as an AxrPose
        const AxrPose* toRaw() const {
            return reinterpret_cast<const AxrPose*>(this);
        }

        /// Get a handle to the Pose as an AxrPose
        /// @returns This as an AxrPose
        AxrPose* toRaw() {
            return reinterpret_cast<AxrPose*>(this);
        }
    };

    static_assert(
        sizeof(AxrPose) == sizeof(axr::Pose),
        "Original type and wrapper have different size!"
    );
}
