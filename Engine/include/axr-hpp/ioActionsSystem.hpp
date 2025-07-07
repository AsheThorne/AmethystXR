#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionsSystem.h"

namespace axr {
    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Bool input action enum
    enum class BoolInputActionEnum {
        Undefined = AXR_BOOL_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 16 ----
        MouseStart = AXR_BOOL_INPUT_ACTION_MOUSE_START,
        MouseClickL = AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L,
        MouseClickR = AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R,
        MouseClickM = AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M,
        MouseClickX1 = AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1,
        MouseClickX2 = AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2,
        MouseDoubleClickL = AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L,
        MouseDoubleClickR = AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R,
        MouseDoubleClickM = AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M,
        MouseDoubleClickX1 = AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1,
        MouseDoubleClickX2 = AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2,
        MouseEnd = AXR_BOOL_INPUT_ACTION_MOUSE_END,
    };

    /// Float input action enum
    enum class FloatInputActionEnum {
        Undefined = AXR_FLOAT_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 8 ----
        MouseStart = AXR_FLOAT_INPUT_ACTION_MOUSE_START,
        MouseWheelUp = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_UP,
        MouseWheelDown = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_DOWN,
        MouseWheelHorizontalUp = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL_UP,
        MouseWheelHorizontalDown = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL_DOWN,
        MouseEnd = AXR_FLOAT_INPUT_ACTION_MOUSE_END,
    };

    /// Vec2 input action enum
    enum class Vec2InputActionEnum {
        Undefined = AXR_VEC2_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 4 ----
        MouseStart = AXR_VEC2_INPUT_ACTION_MOUSE_START,
        MouseMoved = AXR_VEC2_INPUT_ACTION_MOUSE_MOVED,
        MousePosition = AXR_VEC2_INPUT_ACTION_MOUSE_POSITION,
        MouseEnd = AXR_VEC2_INPUT_ACTION_MOUSE_END,
    };

    // ----------------------------------------- //
    // Input/Output Actions System Definition
    // ----------------------------------------- //

    /// Input/Output Actions System
    class IOActionsSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param ioActionsSystem Input/Output actions system handle
        IOActionsSystem(const AxrIOActionsSystem_T ioActionsSystem):
            m_IOActionsSystem(ioActionsSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrIOActionsSystem_T m_IOActionsSystem;
    };
}
