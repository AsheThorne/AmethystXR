#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Bool input action enum
enum AxrBoolInputActionEnum {
    AXR_BOOL_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 16 ----
    AXR_BOOL_INPUT_ACTION_MOUSE_START = 1,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L = 1,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R = 2,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M = 3,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1 = 4,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2 = 5,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L = 6,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R = 7,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M = 8,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1 = 9,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2 = 10,
    AXR_BOOL_INPUT_ACTION_MOUSE_END = 16,
};

/// Float input action enum
enum AxrFloatInputActionEnum {
    AXR_FLOAT_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 8 ----
    AXR_FLOAT_INPUT_ACTION_MOUSE_START = 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_UP = 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_DOWN = 2,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL_UP = 3,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL_DOWN = 4,
    AXR_FLOAT_INPUT_ACTION_MOUSE_END = 8,
};

/// Vec2 input action enum
enum AxrVec2InputActionEnum {
    AXR_VEC2_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 4 ----
    AXR_VEC2_INPUT_ACTION_MOUSE_START = 1,
    AXR_VEC2_INPUT_ACTION_MOUSE_MOVED = 1,
    AXR_VEC2_INPUT_ACTION_MOUSE_POSITION = 2,
    AXR_VEC2_INPUT_ACTION_MOUSE_END = 4,
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrInputSystem Handle
typedef class AxrInputSystem* AxrInputSystem_T;
/// const AxrInputSystem Handle
typedef const AxrInputSystem* AxrInputSystemConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
}
