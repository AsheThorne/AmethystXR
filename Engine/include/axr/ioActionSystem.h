#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/types.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Input/Output action xr visibility enum
///
/// Auto -> Only if there's an action with xr binding, make it available for an xr session to use and modify.
///
/// Always -> Even if there's no action with an xr binding, make it available for an xr session to use and modify.
/// This can be useful if the action is meant for desktop use, but an xr session is allowed to map its own inputs to it.
///
/// Never -> Never make this available to an xr session to use or modify.
/// Warning: Even if there's an action with an xr binding attached, it'll never work for any xr binding.
enum AxrIOActionXrVisibilityEnum {
    AXR_IO_ACTION_XR_VISIBILITY_AUTO = 0,
    AXR_IO_ACTION_XR_VISIBILITY_ALWAYS,
    AXR_IO_ACTION_XR_VISIBILITY_NEVER,
};

/// Xr interaction profile enum
enum AxrXrInteractionProfileEnum {
    AXR_XR_INTERACTION_PROFILE_UNDEFINED = 0,
    /// https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#_khronos_simple_controller_profile
    AXR_XR_INTERACTION_PROFILE_SIMPLE_CONTROLLER,
    /// https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#_valve_index_controller_profile
    AXR_XR_INTERACTION_PROFILE_VALVE_INDEX_CONTROLLER,
};

/// Bool input action enum
enum AxrBoolInputActionEnum {
    AXR_BOOL_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 16 ----
    AXR_BOOL_INPUT_ACTION_MOUSE_START = AXR_BOOL_INPUT_ACTION_UNDEFINED + 1,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L = AXR_BOOL_INPUT_ACTION_MOUSE_START + 0,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R = AXR_BOOL_INPUT_ACTION_MOUSE_START + 1,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M = AXR_BOOL_INPUT_ACTION_MOUSE_START + 2,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1 = AXR_BOOL_INPUT_ACTION_MOUSE_START + 3,
    AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2 = AXR_BOOL_INPUT_ACTION_MOUSE_START + 4,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L = AXR_BOOL_INPUT_ACTION_MOUSE_START + 5,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R = AXR_BOOL_INPUT_ACTION_MOUSE_START + 6,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M = AXR_BOOL_INPUT_ACTION_MOUSE_START + 7,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1 = AXR_BOOL_INPUT_ACTION_MOUSE_START + 8,
    AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2 = AXR_BOOL_INPUT_ACTION_MOUSE_START + 9,
    AXR_BOOL_INPUT_ACTION_MOUSE_END = AXR_BOOL_INPUT_ACTION_MOUSE_START + 15,

    // ---- Keyboard - Max of 128 ----
    AXR_BOOL_INPUT_ACTION_KEYBOARD_START = AXR_BOOL_INPUT_ACTION_MOUSE_END + 1,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_BACKSPACE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 0,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_TAB = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 1,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_ENTER = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 2,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_SHIFT = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 3,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_CTRL = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 4,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_ALT = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 5,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PAUSE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 6,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_CAPSLOCK = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 7,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_ESCAPE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 8,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_SPACE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 9,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_UP = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 10,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_DOWN = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 11,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_END_KEY = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 12,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_HOME_KEY = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 13,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_LEFT_ARROW = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 14,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_UP_ARROW = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 15,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_RIGHT_ARROW = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 16,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_DOWN_ARROW = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 17,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PRINT_SCREEN = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 18,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_INSERT = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 19,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_DELETE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 20,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_0 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 21,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 22,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 23,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 24,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 25,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 26,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 27,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 28,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 29,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 30,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_A = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 31,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_B = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 32,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_C = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 33,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_D = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 34,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_E = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 35,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 36,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_G = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 37,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_H = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 38,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_I = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 39,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_J = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 40,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_K = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 41,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_L = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 42,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_M = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 43,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_N = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 44,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_O = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 45,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_P = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 46,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_Q = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 47,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_R = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 48,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_S = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 49,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_T = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 50,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_U = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 51,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_V = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 52,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_W = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 53,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_X = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 54,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_Y = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 55,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_Z = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 56,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_L = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 57,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_R = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 58,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_0 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 59,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 60,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 61,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 62,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 63,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 64,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 65,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 66,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 67,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 68,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 69,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 70,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 71,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 72,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 73,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 74,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 75,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 76,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 77,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F10 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 78,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F11 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 79,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_F12 = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 80,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_NUM_LOCK = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 81,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_SCROLL_LOCK = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 82,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_SUBTRACT = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 83,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_DECIMAL = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 84,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_DIVIDE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 85,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PLUS = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 86,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_COMMA = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 87,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_MINUS = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 88,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_PERIOD = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 89,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM1_SEMICOLON_COLON = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 90,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM2_FORWARD_SLASH_QUESTION = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 91,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM3_BACKTICK_TILDE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 92,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM4_OPEN_SQUARE_BRACKET_BRACES = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 93,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM5_BACKSLASH_PIPE = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 94,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM6_CLOSED_SQUARE_BRACKET_BRACES = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 95,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM7_QUOTES = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 96,
    AXR_BOOL_INPUT_ACTION_KEYBOARD_END = AXR_BOOL_INPUT_ACTION_KEYBOARD_START + 127,

    // ---- XR - Max of 128 ----
    AXR_BOOL_INPUT_ACTION_XR_START = AXR_BOOL_INPUT_ACTION_KEYBOARD_END + 1,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_SELECT_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 0,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_SELECT_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 1,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_MENU_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 2,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_MENU_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 3,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_A_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 4,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_A_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 5,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_A_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 6,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_A_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 7,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_B_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 8,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_B_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 9,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_B_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 10,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_B_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 11,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 12,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 13,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 14,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 15,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 16,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_CLICK = AXR_BOOL_INPUT_ACTION_XR_START + 17,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 18,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 19,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 20,
    AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_TOUCH = AXR_BOOL_INPUT_ACTION_XR_START + 21,
    AXR_BOOL_INPUT_ACTION_XR_END = AXR_BOOL_INPUT_ACTION_XR_START + 127,
};

/// Float input action enum
enum AxrFloatInputActionEnum {
    AXR_FLOAT_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 4 ----
    AXR_FLOAT_INPUT_ACTION_MOUSE_START = AXR_FLOAT_INPUT_ACTION_UNDEFINED + 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL = AXR_FLOAT_INPUT_ACTION_MOUSE_START + 0,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL = AXR_FLOAT_INPUT_ACTION_MOUSE_START + 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_END = AXR_FLOAT_INPUT_ACTION_MOUSE_START + 3,

    // ---- XR - Max of 128 ----
    AXR_FLOAT_INPUT_ACTION_XR_START = AXR_FLOAT_INPUT_ACTION_MOUSE_END + 1,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_SQUEEZE_VALUE = AXR_FLOAT_INPUT_ACTION_XR_START + 0,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_SQUEEZE_VALUE = AXR_FLOAT_INPUT_ACTION_XR_START + 1,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_SQUEEZE_FORCE = AXR_FLOAT_INPUT_ACTION_XR_START + 2,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_SQUEEZE_FORCE = AXR_FLOAT_INPUT_ACTION_XR_START + 3,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_VALUE = AXR_FLOAT_INPUT_ACTION_XR_START + 4,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_VALUE = AXR_FLOAT_INPUT_ACTION_XR_START + 5,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_X = AXR_FLOAT_INPUT_ACTION_XR_START + 6,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_X = AXR_FLOAT_INPUT_ACTION_XR_START + 7,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_Y = AXR_FLOAT_INPUT_ACTION_XR_START + 8,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_Y = AXR_FLOAT_INPUT_ACTION_XR_START + 9,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_X = AXR_FLOAT_INPUT_ACTION_XR_START + 10,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_X = AXR_FLOAT_INPUT_ACTION_XR_START + 11,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_Y = AXR_FLOAT_INPUT_ACTION_XR_START + 12,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_Y = AXR_FLOAT_INPUT_ACTION_XR_START + 13,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_FORCE = AXR_FLOAT_INPUT_ACTION_XR_START + 14,
    AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_FORCE = AXR_FLOAT_INPUT_ACTION_XR_START + 15,
    AXR_FLOAT_INPUT_ACTION_XR_END = AXR_FLOAT_INPUT_ACTION_XR_START + 127,
};

/// Vec2 input action enum
enum AxrVec2InputActionEnum {
    AXR_VEC2_INPUT_ACTION_UNDEFINED = 0,

    // ---- Mouse - Max of 4 ----
    AXR_VEC2_INPUT_ACTION_MOUSE_START = AXR_VEC2_INPUT_ACTION_UNDEFINED + 1,
    AXR_VEC2_INPUT_ACTION_MOUSE_MOVED = AXR_VEC2_INPUT_ACTION_MOUSE_START + 0,
    AXR_VEC2_INPUT_ACTION_MOUSE_POSITION = AXR_VEC2_INPUT_ACTION_MOUSE_START + 1,
    AXR_VEC2_INPUT_ACTION_MOUSE_END = AXR_VEC2_INPUT_ACTION_MOUSE_START + 3,

    // ---- XR - Max of 128 ----
    AXR_VEC2_INPUT_ACTION_XR_START = AXR_VEC2_INPUT_ACTION_MOUSE_END + 1,
    AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK = AXR_VEC2_INPUT_ACTION_XR_START + 0,
    AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK = AXR_VEC2_INPUT_ACTION_XR_START + 1,
    AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD = AXR_VEC2_INPUT_ACTION_XR_START + 2,
    AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD = AXR_VEC2_INPUT_ACTION_XR_START + 3,
    AXR_VEC2_INPUT_ACTION_XR_END = AXR_VEC2_INPUT_ACTION_XR_START + 127,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Bool Input Action Config
struct AxrBoolInputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
    AxrIOActionXrVisibilityEnum XrVisibility;
    uint32_t BindingCount;
    AxrBoolInputActionEnum* Bindings;
};

/// Float Input Action Config
struct AxrFloatInputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
    AxrIOActionXrVisibilityEnum XrVisibility;
    uint32_t BindingCount;
    AxrFloatInputActionEnum* Bindings;
};

/// Vec2 Input Action Config
struct AxrVec2InputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
    AxrIOActionXrVisibilityEnum XrVisibility;
    uint32_t BindingCount;
    AxrVec2InputActionEnum* Bindings;
};

/// Input/Output action set config
struct AxrIOActionSetConfig {
    char Name[AXR_MAX_IO_ACTION_SET_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE];
    uint32_t BoolInputActionCount;
    AxrBoolInputActionConfig* BoolInputActions;
    uint32_t FloatInputActionCount;
    AxrFloatInputActionConfig* FloatInputActions;
    uint32_t Vec2InputActionCount;
    AxrVec2InputActionConfig* Vec2InputActions;
};

/// Input/Output action system config
struct AxrIOActionSystemConfig {
    uint32_t IOActionSetCount;
    AxrIOActionSetConfig* IOActionSets;
    uint32_t XrInteractionProfileCount;
    AxrXrInteractionProfileEnum* XrInteractionProfiles;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrBoolInputAction Handle
typedef class AxrBoolInputAction* AxrBoolInputAction_T;
/// const AxrBoolInputAction Handle
typedef const AxrBoolInputAction* AxrBoolInputActionConst_T;

/// AxrFloatInputAction Handle
typedef class AxrFloatInputAction* AxrFloatInputAction_T;
/// const AxrFloatInputAction Handle
typedef const AxrFloatInputAction* AxrFloatInputActionConst_T;

/// AxrVec2InputAction Handle
typedef class AxrVec2InputAction* AxrVec2InputAction_T;
/// const AxrVec2InputAction Handle
typedef const AxrVec2InputAction* AxrVec2InputActionConst_T;

/// AxrIOActionSet Handle
typedef class AxrIOActionSet* AxrIOActionSet_T;
/// const AxrIOActionSet Handle
typedef const AxrIOActionSet* AxrIOActionSetConst_T;

/// AxrIOActionSystem Handle
typedef class AxrIOActionSystem* AxrIOActionSystem_T;
/// const AxrIOActionSystem Handle
typedef const AxrIOActionSystem* AxrIOActionSystemConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    // ---- Configs ----

    /// Clone the given bool input action config
    /// @param inputActionConfig Bool input action config to clone
    /// @returns The cloned bool input action
    AXR_API AxrBoolInputActionConfig axrBoolInputActionConfigClone(const AxrBoolInputActionConfig* inputActionConfig);
    /// Destroy the given bool input action config
    /// @param inputActionConfig Bool input action config to destroy
    AXR_API void axrBoolInputActionConfigDestroy(AxrBoolInputActionConfig* inputActionConfig);
    /// Clone the given float input action config
    /// @param inputActionConfig Float input action config to clone
    /// @returns The cloned float input action
    AXR_API AxrFloatInputActionConfig axrFloatInputActionConfigClone(
        const AxrFloatInputActionConfig* inputActionConfig
    );
    /// Destroy the given float input action config
    /// @param inputActionConfig Float input action config to destroy
    AXR_API void axrFloatInputActionConfigDestroy(AxrFloatInputActionConfig* inputActionConfig);
    /// Clone the given vec2 input action config
    /// @param inputActionConfig Vec2 input action config to clone
    /// @returns The cloned vec2 input action
    AXR_API AxrVec2InputActionConfig axrVec2InputActionConfigClone(const AxrVec2InputActionConfig* inputActionConfig);
    /// Destroy the given vec2 input action config
    /// @param inputActionConfig Vec2 input action config to destroy
    AXR_API void axrVec2InputActionConfigDestroy(AxrVec2InputActionConfig* inputActionConfig);

    /// Clone the given input/output action set config
    /// @param ioActionSetConfig Input/Output action set config to clone
    /// @returns The cloned input/output action set
    AXR_API AxrIOActionSetConfig axrIOActionSetConfigClone(const AxrIOActionSetConfig* ioActionSetConfig);
    /// Destroy the given input/output action set config
    /// @param ioActionSetConfig Input/Output action set config to destroy
    AXR_API void axrIOActionSetConfigDestroy(AxrIOActionSetConfig* ioActionSetConfig);

    // ---- Bool Input Action ----

    /// Check if the value has changed since the last frame
    /// @param inputAction Bool input action to use
    /// @returns True if the value has changed since the last frame
    AXR_API bool axrBoolInputActionValueChanged(AxrBoolInputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Bool input action to use
    /// @returns The current value of this input action
    AXR_API bool axrBoolInputActionGetValue(AxrBoolInputActionConst_T inputAction);

    // ---- Float Input Action ----

    /// Check if the value has changed since the last frame
    /// @param inputAction Float input action to use
    /// @returns True if the value has changed since the last frame
    AXR_API bool axrFloatInputActionValueChanged(AxrFloatInputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Float input action to use
    /// @returns The current value of this input action
    AXR_API float axrFloatInputActionGetValue(AxrFloatInputActionConst_T inputAction);

    // ---- Vec2 Input Action ----

    /// Check if the value has changed since the last frame
    /// @param inputAction Vec2 input action to use
    /// @returns True if the value has changed since the last frame
    AXR_API bool axrVec2InputActionValueChanged(AxrVec2InputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Vec2 input action to use
    /// @returns The current value of this input action
    AXR_API AxrVec2 axrVec2InputActionGetValue(AxrVec2InputActionConst_T inputAction);

    // ---- Input/Output Action Set ----

    /// Get the named bool input action
    /// @param ioActionSet Input/Output action set to use
    /// @param name Bool input action name
    /// @returns The bool input action or nullptr if it wasn't found
    AXR_API AxrBoolInputAction_T axrIOActionSetGetBoolInputAction(AxrIOActionSet_T ioActionSet, const char* name);
    /// Get the named float input action
    /// @param ioActionSet Input/Output action set to use
    /// @param name Float input action name
    /// @returns The float input action or nullptr if it wasn't found
    AXR_API AxrFloatInputAction_T axrIOActionSetGetFloatInputAction(AxrIOActionSet_T ioActionSet, const char* name);
    /// Get the named vec2 input action
    /// @param ioActionSet Input/Output action set to use
    /// @param name Vec2 input action name
    /// @returns The vec2 input action or nullptr if it wasn't found
    AXR_API AxrVec2InputAction_T axrIOActionSetGetVec2InputAction(AxrIOActionSet_T ioActionSet, const char* name);

    // ---- Input/Output Action System ----

    /// Get the named input/output action set
    /// @param ioActionSystem Input/Output action system to use
    /// @param name Input/Output action set name
    /// @returns The input/output action set or nullptr if it wasn't found
    AXR_API AxrIOActionSet_T axrIOActionSystemGetIOActionSet(AxrIOActionSystem_T ioActionSystem, const char* name);
}
