#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"
#include "common/types.hpp"

namespace axr {
    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Action xr visibility enum
    ///
    /// Auto -> Only if there's an action with xr binding, make it available for an xr session to use and modify.
    ///
    /// Always -> Even if there's no action with an xr binding, make it available for an xr session to use and modify.
    /// This can be useful if the action is meant for desktop use, but an xr session is allowed to map its own inputs to it.
    ///
    /// Never -> Never make this available to an xr session to use or modify.
    /// Warning: Even if there's an action with an xr binding attached, it'll never work for any xr binding.
    enum class ActionXrVisibilityEnum {
        Auto = AXR_ACTION_XR_VISIBILITY_AUTO,
        Always = AXR_ACTION_XR_VISIBILITY_ALWAYS,
        Never = AXR_ACTION_XR_VISIBILITY_NEVER,
    };

    /// Xr interaction profile enum
    enum class XrInteractionProfileEnum {
        Undefined = AXR_XR_INTERACTION_PROFILE_UNDEFINED,
        /// https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#_khronos_simple_controller_profile
        KhrSimpleController = AXR_XR_INTERACTION_PROFILE_KHR_SIMPLE_CONTROLLER,
        /// https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#_valve_index_controller_profile
        ValveIndexController = AXR_XR_INTERACTION_PROFILE_VALVE_INDEX_CONTROLLER,
    };

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

        // ---- Keyboard - Max of 128 ----
        KeyboardStart = AXR_BOOL_INPUT_ACTION_KEYBOARD_START,
        KeyboardBackspace = AXR_BOOL_INPUT_ACTION_KEYBOARD_BACKSPACE,
        KeyboardTab = AXR_BOOL_INPUT_ACTION_KEYBOARD_TAB,
        KeyboardEnter = AXR_BOOL_INPUT_ACTION_KEYBOARD_ENTER,
        KeyboardShift = AXR_BOOL_INPUT_ACTION_KEYBOARD_SHIFT,
        KeyboardCtrl = AXR_BOOL_INPUT_ACTION_KEYBOARD_CTRL,
        KeyboardAlt = AXR_BOOL_INPUT_ACTION_KEYBOARD_ALT,
        KeyboardPause = AXR_BOOL_INPUT_ACTION_KEYBOARD_PAUSE,
        KeyboardCapslock = AXR_BOOL_INPUT_ACTION_KEYBOARD_CAPSLOCK,
        KeyboardEscape = AXR_BOOL_INPUT_ACTION_KEYBOARD_ESCAPE,
        KeyboardSpace = AXR_BOOL_INPUT_ACTION_KEYBOARD_SPACE,
        KeyboardPageUp = AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_UP,
        KeyboardPageDown = AXR_BOOL_INPUT_ACTION_KEYBOARD_PAGE_DOWN,
        KeyboardEndKey = AXR_BOOL_INPUT_ACTION_KEYBOARD_END_KEY,
        KeyboardHomeKey = AXR_BOOL_INPUT_ACTION_KEYBOARD_HOME_KEY,
        KeyboardLeftArrow = AXR_BOOL_INPUT_ACTION_KEYBOARD_LEFT_ARROW,
        KeyboardUpArrow = AXR_BOOL_INPUT_ACTION_KEYBOARD_UP_ARROW,
        KeyboardRightArrow = AXR_BOOL_INPUT_ACTION_KEYBOARD_RIGHT_ARROW,
        KeyboardDownArrow = AXR_BOOL_INPUT_ACTION_KEYBOARD_DOWN_ARROW,
        KeyboardPrintScreen = AXR_BOOL_INPUT_ACTION_KEYBOARD_PRINT_SCREEN,
        KeyboardInsert = AXR_BOOL_INPUT_ACTION_KEYBOARD_INSERT,
        KeyboardDelete = AXR_BOOL_INPUT_ACTION_KEYBOARD_DELETE,
        Keyboard0 = AXR_BOOL_INPUT_ACTION_KEYBOARD_0,
        Keyboard1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_1,
        Keyboard2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_2,
        Keyboard3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_3,
        Keyboard4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_4,
        Keyboard5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_5,
        Keyboard6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_6,
        Keyboard7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_7,
        Keyboard8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_8,
        Keyboard9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_9,
        KeyboardA = AXR_BOOL_INPUT_ACTION_KEYBOARD_A,
        KeyboardB = AXR_BOOL_INPUT_ACTION_KEYBOARD_B,
        KeyboardC = AXR_BOOL_INPUT_ACTION_KEYBOARD_C,
        KeyboardD = AXR_BOOL_INPUT_ACTION_KEYBOARD_D,
        KeyboardE = AXR_BOOL_INPUT_ACTION_KEYBOARD_E,
        KeyboardF = AXR_BOOL_INPUT_ACTION_KEYBOARD_F,
        KeyboardG = AXR_BOOL_INPUT_ACTION_KEYBOARD_G,
        KeyboardH = AXR_BOOL_INPUT_ACTION_KEYBOARD_H,
        KeyboardI = AXR_BOOL_INPUT_ACTION_KEYBOARD_I,
        KeyboardJ = AXR_BOOL_INPUT_ACTION_KEYBOARD_J,
        KeyboardK = AXR_BOOL_INPUT_ACTION_KEYBOARD_K,
        KeyboardL = AXR_BOOL_INPUT_ACTION_KEYBOARD_L,
        KeyboardM = AXR_BOOL_INPUT_ACTION_KEYBOARD_M,
        KeyboardN = AXR_BOOL_INPUT_ACTION_KEYBOARD_N,
        KeyboardO = AXR_BOOL_INPUT_ACTION_KEYBOARD_O,
        KeyboardP = AXR_BOOL_INPUT_ACTION_KEYBOARD_P,
        KeyboardQ = AXR_BOOL_INPUT_ACTION_KEYBOARD_Q,
        KeyboardR = AXR_BOOL_INPUT_ACTION_KEYBOARD_R,
        KeyboardS = AXR_BOOL_INPUT_ACTION_KEYBOARD_S,
        KeyboardT = AXR_BOOL_INPUT_ACTION_KEYBOARD_T,
        KeyboardU = AXR_BOOL_INPUT_ACTION_KEYBOARD_U,
        KeyboardV = AXR_BOOL_INPUT_ACTION_KEYBOARD_V,
        KeyboardW = AXR_BOOL_INPUT_ACTION_KEYBOARD_W,
        KeyboardX = AXR_BOOL_INPUT_ACTION_KEYBOARD_X,
        KeyboardY = AXR_BOOL_INPUT_ACTION_KEYBOARD_Y,
        KeyboardZ = AXR_BOOL_INPUT_ACTION_KEYBOARD_Z,
        KeyboardWinL = AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_L,
        KeyboardWinR = AXR_BOOL_INPUT_ACTION_KEYBOARD_WIN_R,
        KeyboardNumberPad0 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_0,
        KeyboardNumberPad1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_1,
        KeyboardNumberPad2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_2,
        KeyboardNumberPad3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_3,
        KeyboardNumberPad4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_4,
        KeyboardNumberPad5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_5,
        KeyboardNumberPad6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_6,
        KeyboardNumberPad7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_7,
        KeyboardNumberPad8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_8,
        KeyboardNumberPad9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUMBER_PAD_9,
        KeyboardF1 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F1,
        KeyboardF2 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F2,
        KeyboardF3 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F3,
        KeyboardF4 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F4,
        KeyboardF5 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F5,
        KeyboardF6 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F6,
        KeyboardF7 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F7,
        KeyboardF8 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F8,
        KeyboardF9 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F9,
        KeyboardF10 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F10,
        KeyboardF11 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F11,
        KeyboardF12 = AXR_BOOL_INPUT_ACTION_KEYBOARD_F12,
        KeyboardNumLock = AXR_BOOL_INPUT_ACTION_KEYBOARD_NUM_LOCK,
        KeyboardScrollLock = AXR_BOOL_INPUT_ACTION_KEYBOARD_SCROLL_LOCK,
        KeyboardSubtract = AXR_BOOL_INPUT_ACTION_KEYBOARD_SUBTRACT,
        KeyboardDecimal = AXR_BOOL_INPUT_ACTION_KEYBOARD_DECIMAL,
        KeyboardDivide = AXR_BOOL_INPUT_ACTION_KEYBOARD_DIVIDE,
        KeyboardPlus = AXR_BOOL_INPUT_ACTION_KEYBOARD_PLUS,
        KeyboardComma = AXR_BOOL_INPUT_ACTION_KEYBOARD_COMMA,
        KeyboardMinus = AXR_BOOL_INPUT_ACTION_KEYBOARD_MINUS,
        KeyboardPeriod = AXR_BOOL_INPUT_ACTION_KEYBOARD_PERIOD,
        KeyboardOem1_Semicolon_Colon = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM1_SEMICOLON_COLON,
        KeyboardOem2_ForwardSlash_Question = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM2_FORWARD_SLASH_QUESTION,
        KeyboardOem3_Backtick_Tilde = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM3_BACKTICK_TILDE,
        KeyboardOem4_OpenSquareBracket_Braces = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM4_OPEN_SQUARE_BRACKET_BRACES,
        KeyboardOem5_Backslash_Pipe = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM5_BACKSLASH_PIPE,
        KeyboardOem6_ClosedSquareBracket_Braces = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM6_CLOSED_SQUARE_BRACKET_BRACES,
        KeyboardOem7_Quotes = AXR_BOOL_INPUT_ACTION_KEYBOARD_OEM7_QUOTES,
        KeyboardEnd = AXR_BOOL_INPUT_ACTION_KEYBOARD_END,

        // ---- XR - Max of 128 ----
        XrStart = AXR_BOOL_INPUT_ACTION_XR_START,
        XrController_Left_Select_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_SELECT_CLICK,
        XrController_Right_Select_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_SELECT_CLICK,
        XrController_Left_Menu_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_MENU_CLICK,
        XrController_Right_Menu_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_MENU_CLICK,
        XrController_Left_A_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_A_CLICK,
        XrController_Right_A_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_A_CLICK,
        XrController_Left_A_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_A_TOUCH,
        XrController_Right_A_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_A_TOUCH,
        XrController_Left_B_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_B_CLICK,
        XrController_Right_B_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_B_CLICK,
        XrController_Left_B_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_B_TOUCH,
        XrController_Right_B_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_B_TOUCH,
        XrController_Left_Trigger_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_CLICK,
        XrController_Right_Trigger_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_CLICK,
        XrController_Left_Trigger_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_TOUCH,
        XrController_Right_Trigger_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_TOUCH,
        XrController_Left_Thumbstick_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_CLICK,
        XrController_Right_Thumbstick_Click = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_CLICK,
        XrController_Left_Thumbstick_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_TOUCH,
        XrController_Right_Thumbstick_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_TOUCH,
        XrController_Left_Trackpad_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_TOUCH,
        XrController_Right_Trackpad_Touch = AXR_BOOL_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_TOUCH,
        XrEnd = AXR_BOOL_INPUT_ACTION_XR_END,
    };

    /// Float input action enum
    enum class FloatInputActionEnum {
        Undefined = AXR_FLOAT_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 4 ----
        MouseStart = AXR_FLOAT_INPUT_ACTION_MOUSE_START,
        MouseWheel = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL,
        MouseWheelHorizontal = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL,
        MouseEnd = AXR_FLOAT_INPUT_ACTION_MOUSE_END,

        // ---- XR - Max of 128 ----
        XrStart = AXR_FLOAT_INPUT_ACTION_XR_START,
        XrController_Left_Squeeze_Value = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_SQUEEZE_VALUE,
        XrController_Right_Squeeze_Value = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_SQUEEZE_VALUE,
        XrController_Left_Squeeze_Force = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_SQUEEZE_FORCE,
        XrController_Right_Squeeze_Force = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_SQUEEZE_FORCE,
        XrController_Left_Trigger_Value = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRIGGER_VALUE,
        XrController_Right_Trigger_Value = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRIGGER_VALUE,
        XrController_Left_Thumbstick_X = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_X,
        XrController_Right_Thumbstick_X = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_X,
        XrController_Left_Thumbstick_Y = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK_Y,
        XrController_Right_Thumbstick_Y = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK_Y,
        XrController_Left_Trackpad_X = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_X,
        XrController_Right_Trackpad_X = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_X,
        XrController_Left_Trackpad_Y = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_Y,
        XrController_Right_Trackpad_Y = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_Y,
        XrController_Left_Trackpad_Force = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD_FORCE,
        XrController_Right_Trackpad_Force = AXR_FLOAT_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD_FORCE,
        XrEnd = AXR_FLOAT_INPUT_ACTION_XR_END,
    };

    /// Vec2 input action enum
    enum class Vec2InputActionEnum {
        Undefined = AXR_VEC2_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 4 ----
        MouseStart = AXR_VEC2_INPUT_ACTION_MOUSE_START,
        MouseMoved = AXR_VEC2_INPUT_ACTION_MOUSE_MOVED,
        MousePosition = AXR_VEC2_INPUT_ACTION_MOUSE_POSITION,
        MouseEnd = AXR_VEC2_INPUT_ACTION_MOUSE_END,

        // ---- XR - Max of 128 ----
        XrStart = AXR_VEC2_INPUT_ACTION_XR_START,
        XrController_Left_Thumbstick = AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_LEFT_THUMBSTICK,
        XrController_Right_Thumbstick = AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_RIGHT_THUMBSTICK,
        XrController_Left_Trackpad = AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_LEFT_TRACKPAD,
        XrController_Right_Trackpad = AXR_VEC2_INPUT_ACTION_XR_CONTROLLER_RIGHT_TRACKPAD,
        XrEnd = AXR_VEC2_INPUT_ACTION_XR_END,
    };

    /// Pose input action enum
    enum class PoseInputActionEnum {
        Undefined = AXR_POSE_INPUT_ACTION_UNDEFINED,

        // ---- XR - Max of 128 ----
        XrStart = AXR_POSE_INPUT_ACTION_XR_START,
        XrHMD = AXR_POSE_INPUT_ACTION_XR_HMD,
        XrController_Left_Grip = AXR_POSE_INPUT_ACTION_XR_CONTROLLER_LEFT_GRIP,
        XrController_Right_Grip = AXR_POSE_INPUT_ACTION_XR_CONTROLLER_RIGHT_GRIP,
        XrController_Left_Aim = AXR_POSE_INPUT_ACTION_XR_CONTROLLER_LEFT_AIM,
        XrController_Right_Aim = AXR_POSE_INPUT_ACTION_XR_CONTROLLER_RIGHT_AIM,
        XrEnd = AXR_POSE_INPUT_ACTION_XR_END,
    };

    /// Haptic output action enum
    enum class HapticOutputActionEnum {
        Undefined = AXR_HAPTIC_OUTPUT_ACTION_UNDEFINED,

        // ---- XR - Max of 128 ----
        XrStart = AXR_HAPTIC_OUTPUT_ACTION_XR_START,
        XrController_Left = AXR_HAPTIC_OUTPUT_ACTION_XR_CONTROLLER_LEFT,
        XrController_Right = AXR_HAPTIC_OUTPUT_ACTION_XR_CONTROLLER_RIGHT,
        XrEnd = AXR_HAPTIC_OUTPUT_ACTION_XR_END,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Bool input action config
    struct BoolInputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        axr::ActionXrVisibilityEnum XrVisibility = {};
        uint32_t BindingCount = 0;
        axr::BoolInputActionEnum* Bindings = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        BoolInputActionConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        BoolInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        /// @param bindings Bool bindings
        BoolInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility,
            const std::vector<axr::BoolInputActionEnum>& bindings
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source BoolInputActionConfig
        explicit BoolInputActionConfig(AxrBoolInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = static_cast<axr::ActionXrVisibilityEnum>(src.XrVisibility);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::BoolInputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source BoolInputActionConfig to copy from
        BoolInputActionConfig(const BoolInputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source BoolInputActionConfig to move from
        BoolInputActionConfig(BoolInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        // ---- Destructor ----

        ~BoolInputActionConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source BoolInputActionConfig to copy from
        BoolInputActionConfig& operator=(const BoolInputActionConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source BoolInputActionConfig to move from
        BoolInputActionConfig& operator=(BoolInputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.XrVisibility = {};
                src.BindingCount = 0;
                src.Bindings = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the BoolInputActionConfig as an AxrBoolInputActionConfig
        /// @returns This as an AxrBoolInputActionConfig
        const AxrBoolInputActionConfig* toRaw() const {
            return reinterpret_cast<const AxrBoolInputActionConfig*>(this);
        }

        /// Get a handle to the BoolInputActionConfig as an AxrBoolInputActionConfig
        /// @returns This as an AxrBoolInputActionConfig
        AxrBoolInputActionConfig* toRaw() {
            return reinterpret_cast<AxrBoolInputActionConfig*>(this);
        }

        /// Add a bindings
        /// @param bindings Bool bindings
        void addBindings(const std::vector<axr::BoolInputActionEnum>& bindings) {
            addBindings(bindings.size(), bindings.data());
        }

        /// Add a bindings
        /// @param bindingCount Bindings count
        /// @param bindings Bool bindings
        void addBindings(const uint32_t bindingCount, const axr::BoolInputActionEnum* bindings) {
            const uint32_t startingSize = BindingCount;
            resizeBindings(BindingCount + bindingCount);

            for (uint32_t i = 0; i < bindingCount; ++i) {
                Bindings[startingSize + i] = bindings[i];
            }
        }

        /// Add a binding
        /// @param binding Bool binding
        void addBinding(const axr::BoolInputActionEnum binding) {
            resizeBindings(BindingCount + 1);

            Bindings[BindingCount - 1] = binding;
        }

        /// Clear the bindings
        void clearBindings() {
            if (Bindings == nullptr) return;

            delete[] Bindings;
            Bindings = nullptr;
            BindingCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));
            XrVisibility = {};
            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
            if (size == 0) {
                clearBindings();
                return;
            }

            const auto newBindings = new axr::BoolInputActionEnum[size]{};
            for (uint32_t i = 0; i < std::min(BindingCount, size); ++i) {
                newBindings[i] = Bindings[i];
                Bindings[i] = axr::BoolInputActionEnum::Undefined;
            }

            clearBindings();
            Bindings = newBindings;
            BindingCount = size;
        }
    };

    static_assert(
        sizeof(AxrBoolInputActionConfig) == sizeof(axr::BoolInputActionConfig),
        "Original type and wrapper have different size!"
    );

    /// Float input action config
    struct FloatInputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        axr::ActionXrVisibilityEnum XrVisibility = {};
        uint32_t BindingCount = 0;
        axr::FloatInputActionEnum* Bindings = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        FloatInputActionConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        FloatInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        /// @param bindings Float bindings
        FloatInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility,
            const std::vector<axr::FloatInputActionEnum>& bindings
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source FloatInputActionConfig
        explicit FloatInputActionConfig(AxrFloatInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = static_cast<axr::ActionXrVisibilityEnum>(src.XrVisibility);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::FloatInputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source FloatInputActionConfig to copy from
        FloatInputActionConfig(const FloatInputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source FloatInputActionConfig to move from
        FloatInputActionConfig(FloatInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        // ---- Destructor ----

        ~FloatInputActionConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source FloatInputActionConfig to copy from
        FloatInputActionConfig& operator=(const FloatInputActionConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source FloatInputActionConfig to move from
        FloatInputActionConfig& operator=(FloatInputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.XrVisibility = {};
                src.BindingCount = 0;
                src.Bindings = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the FloatInputActionConfig as an AxrFloatInputActionConfig
        /// @returns This as an AxrFloatInputActionConfig
        const AxrFloatInputActionConfig* toRaw() const {
            return reinterpret_cast<const AxrFloatInputActionConfig*>(this);
        }

        /// Get a handle to the FloatInputActionConfig as an AxrFloatInputActionConfig
        /// @returns This as an AxrFloatInputActionConfig
        AxrFloatInputActionConfig* toRaw() {
            return reinterpret_cast<AxrFloatInputActionConfig*>(this);
        }

        /// Add a bindings
        /// @param bindings Float bindings
        void addBindings(const std::vector<axr::FloatInputActionEnum>& bindings) {
            addBindings(bindings.size(), bindings.data());
        }

        /// Add a bindings
        /// @param bindingCount Bindings count
        /// @param bindings Float bindings
        void addBindings(const uint32_t bindingCount, const axr::FloatInputActionEnum* bindings) {
            const uint32_t startingSize = BindingCount;
            resizeBindings(BindingCount + bindingCount);

            for (uint32_t i = 0; i < bindingCount; ++i) {
                Bindings[startingSize + i] = bindings[i];
            }
        }

        /// Add a binding
        /// @param binding Float binding
        void addBinding(const axr::FloatInputActionEnum binding) {
            resizeBindings(BindingCount + 1);

            Bindings[BindingCount - 1] = binding;
        }

        /// Clear the bindings
        void clearBindings() {
            if (Bindings == nullptr) return;

            delete[] Bindings;
            Bindings = nullptr;
            BindingCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));
            XrVisibility = {};

            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
            if (size == 0) {
                clearBindings();
                return;
            }

            const auto newBindings = new axr::FloatInputActionEnum[size]{};
            for (uint32_t i = 0; i < std::min(BindingCount, size); ++i) {
                newBindings[i] = Bindings[i];
                Bindings[i] = axr::FloatInputActionEnum::Undefined;
            }

            clearBindings();
            Bindings = newBindings;
            BindingCount = size;
        }
    };

    static_assert(
        sizeof(AxrFloatInputActionConfig) == sizeof(axr::FloatInputActionConfig),
        "Original type and wrapper have different size!"
    );

    /// Vec2 input action config
    struct Vec2InputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        axr::ActionXrVisibilityEnum XrVisibility = {};
        uint32_t BindingCount = 0;
        axr::Vec2InputActionEnum* Bindings = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Vec2InputActionConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        Vec2InputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        /// @param bindings Vec2 bindings
        Vec2InputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility,
            const std::vector<axr::Vec2InputActionEnum>& bindings
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source Vec2InputActionConfig
        explicit Vec2InputActionConfig(AxrVec2InputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = static_cast<axr::ActionXrVisibilityEnum>(src.XrVisibility);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::Vec2InputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source Vec2InputActionConfig to copy from
        Vec2InputActionConfig(const Vec2InputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source Vec2InputActionConfig to move from
        Vec2InputActionConfig(Vec2InputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        // ---- Destructor ----

        ~Vec2InputActionConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Vec2InputActionConfig to copy from
        Vec2InputActionConfig& operator=(const Vec2InputActionConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Vec2InputActionConfig to move from
        Vec2InputActionConfig& operator=(Vec2InputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.XrVisibility = {};
                src.BindingCount = 0;
                src.Bindings = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Vec2InputActionConfig as an AxrVec2InputActionConfig
        /// @returns This as an AxrVec2InputActionConfig
        const AxrVec2InputActionConfig* toRaw() const {
            return reinterpret_cast<const AxrVec2InputActionConfig*>(this);
        }

        /// Get a handle to the Vec2InputActionConfig as an AxrVec2InputActionConfig
        /// @returns This as an AxrVec2InputActionConfig
        AxrVec2InputActionConfig* toRaw() {
            return reinterpret_cast<AxrVec2InputActionConfig*>(this);
        }

        /// Add a bindings
        /// @param bindings Vec2 bindings
        void addBindings(const std::vector<axr::Vec2InputActionEnum>& bindings) {
            addBindings(bindings.size(), bindings.data());
        }

        /// Add a bindings
        /// @param bindingCount Bindings count
        /// @param bindings Vec2 bindings
        void addBindings(const uint32_t bindingCount, const axr::Vec2InputActionEnum* bindings) {
            const uint32_t startingSize = BindingCount;
            resizeBindings(BindingCount + bindingCount);

            for (uint32_t i = 0; i < bindingCount; ++i) {
                Bindings[startingSize + i] = bindings[i];
            }
        }

        /// Add a binding
        /// @param binding vec2 binding
        void addBinding(const axr::Vec2InputActionEnum binding) {
            resizeBindings(BindingCount + 1);

            Bindings[BindingCount - 1] = binding;
        }

        /// Clear the bindings
        void clearBindings() {
            if (Bindings == nullptr) return;

            delete[] Bindings;
            Bindings = nullptr;
            BindingCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));
            XrVisibility = {};

            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
            if (size == 0) {
                clearBindings();
                return;
            }

            const auto newBindings = new axr::Vec2InputActionEnum[size]{};
            for (uint32_t i = 0; i < std::min(BindingCount, size); ++i) {
                newBindings[i] = Bindings[i];
                Bindings[i] = axr::Vec2InputActionEnum::Undefined;
            }

            clearBindings();
            Bindings = newBindings;
            BindingCount = size;
        }
    };

    static_assert(
        sizeof(AxrVec2InputActionConfig) == sizeof(axr::Vec2InputActionConfig),
        "Original type and wrapper have different size!"
    );

    /// Pose input action config
    struct PoseInputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        axr::ActionXrVisibilityEnum XrVisibility = {};
        axr::PoseInputActionEnum Binding = axr::PoseInputActionEnum::Undefined;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        PoseInputActionConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        PoseInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        /// @param binding Pose binding
        PoseInputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility,
            const axr::PoseInputActionEnum binding
        ): XrVisibility(xrVisibility),
            Binding(binding) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param src Source PoseInputActionConfig
        explicit PoseInputActionConfig(AxrPoseInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = static_cast<axr::ActionXrVisibilityEnum>(src.XrVisibility);
            Binding = static_cast<axr::PoseInputActionEnum>(src.Binding);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.Binding = AXR_POSE_INPUT_ACTION_UNDEFINED;
        }

        /// Copy Constructor
        /// @param src Source PoseInputActionConfig to copy from
        PoseInputActionConfig(const PoseInputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            Binding = src.Binding;
        }

        /// Move Constructor
        /// @param src Source PoseInputActionConfig to move from
        PoseInputActionConfig(PoseInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            Binding = src.Binding;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.Binding = axr::PoseInputActionEnum::Undefined;
        }

        // ---- Destructor ----

        ~PoseInputActionConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source PoseInputActionConfig to copy from
        PoseInputActionConfig& operator=(const PoseInputActionConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                Binding = src.Binding;
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source PoseInputActionConfig to move from
        PoseInputActionConfig& operator=(PoseInputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                Binding = src.Binding;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.XrVisibility = {};
                src.Binding = axr::PoseInputActionEnum::Undefined;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the PoseInputActionConfig as an AxrPoseInputActionConfig
        /// @returns This as an AxrPoseInputActionConfig
        const AxrPoseInputActionConfig* toRaw() const {
            return reinterpret_cast<const AxrPoseInputActionConfig*>(this);
        }

        /// Get a handle to the PoseInputActionConfig as an AxrPoseInputActionConfig
        /// @returns This as an AxrPoseInputActionConfig
        AxrPoseInputActionConfig* toRaw() {
            return reinterpret_cast<AxrPoseInputActionConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));
            XrVisibility = {};
            Binding = axr::PoseInputActionEnum::Undefined;
        }
    };

    static_assert(
        sizeof(AxrPoseInputActionConfig) == sizeof(axr::PoseInputActionConfig),
        "Original type and wrapper have different size!"
    );

    /// Haptic output action config
    struct HapticOutputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        axr::ActionXrVisibilityEnum XrVisibility = {};
        uint32_t BindingCount = 0;
        axr::HapticOutputActionEnum* Bindings = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        HapticOutputActionConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        HapticOutputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param xrVisibility Xr visibility
        /// @param bindings Haptic bindings
        HapticOutputActionConfig(
            const char* name,
            const char* localizedName,
            const axr::ActionXrVisibilityEnum xrVisibility,
            const std::vector<axr::HapticOutputActionEnum>& bindings
        ): XrVisibility(xrVisibility) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source HapticOutputActionConfig
        explicit HapticOutputActionConfig(AxrHapticOutputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = static_cast<axr::ActionXrVisibilityEnum>(src.XrVisibility);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::HapticOutputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source HapticOutputActionConfig to copy from
        HapticOutputActionConfig(const HapticOutputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source HapticOutputActionConfig to move from
        HapticOutputActionConfig(HapticOutputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
            XrVisibility = src.XrVisibility;
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.XrVisibility = {};
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        // ---- Destructor ----

        ~HapticOutputActionConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source HapticOutputActionConfig to copy from
        HapticOutputActionConfig& operator=(const HapticOutputActionConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source HapticOutputActionConfig to move from
        HapticOutputActionConfig& operator=(HapticOutputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_LOCALIZED_NAME_SIZE);
                XrVisibility = src.XrVisibility;
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.XrVisibility = {};
                src.BindingCount = 0;
                src.Bindings = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the HapticOutputActionConfig as an AxrHapticOutputActionConfig
        /// @returns This as an AxrHapticOutputActionConfig
        const AxrHapticOutputActionConfig* toRaw() const {
            return reinterpret_cast<const AxrHapticOutputActionConfig*>(this);
        }

        /// Get a handle to the HapticOutputActionConfig as an AxrHapticOutputActionConfig
        /// @returns This as an AxrHapticOutputActionConfig
        AxrHapticOutputActionConfig* toRaw() {
            return reinterpret_cast<AxrHapticOutputActionConfig*>(this);
        }

        /// Add a bindings
        /// @param bindings Haptic bindings
        void addBindings(const std::vector<axr::HapticOutputActionEnum>& bindings) {
            addBindings(bindings.size(), bindings.data());
        }

        /// Add a bindings
        /// @param bindingCount Bindings count
        /// @param bindings Haptic bindings
        void addBindings(const uint32_t bindingCount, const axr::HapticOutputActionEnum* bindings) {
            const uint32_t startingSize = BindingCount;
            resizeBindings(BindingCount + bindingCount);

            for (uint32_t i = 0; i < bindingCount; ++i) {
                Bindings[startingSize + i] = bindings[i];
            }
        }

        /// Add a binding
        /// @param binding Haptic binding
        void addBinding(const axr::HapticOutputActionEnum binding) {
            resizeBindings(BindingCount + 1);

            Bindings[BindingCount - 1] = binding;
        }

        /// Clear the bindings
        void clearBindings() {
            if (Bindings == nullptr) return;

            delete[] Bindings;
            Bindings = nullptr;
            BindingCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));
            XrVisibility = {};

            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
            if (size == 0) {
                clearBindings();
                return;
            }

            const auto newBindings = new axr::HapticOutputActionEnum[size]{};
            for (uint32_t i = 0; i < std::min(BindingCount, size); ++i) {
                newBindings[i] = Bindings[i];
                Bindings[i] = axr::HapticOutputActionEnum::Undefined;
            }

            clearBindings();
            Bindings = newBindings;
            BindingCount = size;
        }
    };

    static_assert(
        sizeof(AxrHapticOutputActionConfig) == sizeof(axr::HapticOutputActionConfig),
        "Original type and wrapper have different size!"
    );

    /// Action set config
    struct ActionSetConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_ACTION_LOCALIZED_NAME_SIZE]{};
        uint32_t BoolInputActionCount = 0;
        axr::BoolInputActionConfig* BoolInputActions = nullptr;
        uint32_t FloatInputActionCount = 0;
        axr::FloatInputActionConfig* FloatInputActions = nullptr;
        uint32_t Vec2InputActionCount = 0;
        axr::Vec2InputActionConfig* Vec2InputActions = nullptr;
        uint32_t PoseInputActionCount = 0;
        axr::PoseInputActionConfig* PoseInputActions = nullptr;
        uint32_t HapticOutputActionCount = 0;
        axr::HapticOutputActionConfig* HapticOutputActions = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ActionSetConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        ActionSetConfig(
            const char* name,
            const char* localizedName
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_SET_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param boolInputActions Bool input actions
        /// @param floatInputActions Float input actions
        /// @param vec2InputActions Vec2 input actions
        /// @param poseInputActions Pose input actions
        /// @param hapticOutputActions Haptic Output actions
        ActionSetConfig(
            const char* name,
            const char* localizedName,
            const std::vector<axr::BoolInputActionConfig>& boolInputActions,
            const std::vector<axr::FloatInputActionConfig>& floatInputActions,
            const std::vector<axr::Vec2InputActionConfig>& vec2InputActions,
            const std::vector<axr::PoseInputActionConfig>& poseInputActions,
            const std::vector<axr::HapticOutputActionConfig>& hapticOutputActions
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_ACTION_SET_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
            }

            addBoolInputActions(boolInputActions);
            addFloatInputActions(floatInputActions);
            addVec2InputActions(vec2InputActions);
            addPoseInputActions(poseInputActions);
            addHapticOutputActions(hapticOutputActions);
        }

        /// Constructor
        /// @param src Source ActionSetConfig
        explicit ActionSetConfig(AxrActionSetConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
            BoolInputActionCount = src.BoolInputActionCount;
            BoolInputActions = reinterpret_cast<axr::BoolInputActionConfig*>(src.BoolInputActions);
            FloatInputActionCount = src.FloatInputActionCount;
            FloatInputActions = reinterpret_cast<axr::FloatInputActionConfig*>(src.FloatInputActions);
            Vec2InputActionCount = src.Vec2InputActionCount;
            Vec2InputActions = reinterpret_cast<axr::Vec2InputActionConfig*>(src.Vec2InputActions);
            PoseInputActionCount = src.PoseInputActionCount;
            PoseInputActions = reinterpret_cast<axr::PoseInputActionConfig*>(src.PoseInputActions);
            HapticOutputActionCount = src.HapticOutputActionCount;
            HapticOutputActions = reinterpret_cast<axr::HapticOutputActionConfig*>(src.HapticOutputActions);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BoolInputActionCount = 0;
            src.BoolInputActions = nullptr;
            src.FloatInputActionCount = 0;
            src.FloatInputActions = nullptr;
            src.Vec2InputActionCount = 0;
            src.Vec2InputActions = nullptr;
            src.PoseInputActionCount = 0;
            src.PoseInputActions = nullptr;
            src.HapticOutputActionCount = 0;
            src.HapticOutputActions = nullptr;
        }

        /// Copy Constructor
        /// @param src Source ActionSetConfig to copy from
        ActionSetConfig(const ActionSetConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
            addBoolInputActions(src.BoolInputActionCount, src.BoolInputActions);
            addFloatInputActions(src.FloatInputActionCount, src.FloatInputActions);
            addVec2InputActions(src.Vec2InputActionCount, src.Vec2InputActions);
            addPoseInputActions(src.PoseInputActionCount, src.PoseInputActions);
            addHapticOutputActions(src.HapticOutputActionCount, src.HapticOutputActions);
        }

        /// Move Constructor
        /// @param src Source ActionSetConfig to move from
        ActionSetConfig(ActionSetConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
            BoolInputActionCount = src.BoolInputActionCount;
            BoolInputActions = src.BoolInputActions;
            FloatInputActionCount = src.FloatInputActionCount;
            FloatInputActions = src.FloatInputActions;
            Vec2InputActionCount = src.Vec2InputActionCount;
            Vec2InputActions = src.Vec2InputActions;
            PoseInputActionCount = src.PoseInputActionCount;
            PoseInputActions = src.PoseInputActions;
            HapticOutputActionCount = src.HapticOutputActionCount;
            HapticOutputActions = src.HapticOutputActions;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BoolInputActionCount = 0;
            src.BoolInputActions = nullptr;
            src.FloatInputActionCount = 0;
            src.FloatInputActions = nullptr;
            src.Vec2InputActionCount = 0;
            src.Vec2InputActions = nullptr;
            src.PoseInputActionCount = 0;
            src.PoseInputActions = nullptr;
            src.HapticOutputActionCount = 0;
            src.HapticOutputActions = nullptr;
        }

        // ---- Destructor ----

        ~ActionSetConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ActionSetConfig to copy from
        ActionSetConfig& operator=(const ActionSetConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
                addBoolInputActions(src.BoolInputActionCount, src.BoolInputActions);
                addFloatInputActions(src.FloatInputActionCount, src.FloatInputActions);
                addVec2InputActions(src.Vec2InputActionCount, src.Vec2InputActions);
                addPoseInputActions(src.PoseInputActionCount, src.PoseInputActions);
                addHapticOutputActions(src.HapticOutputActionCount, src.HapticOutputActions);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ActionSetConfig to move from
        ActionSetConfig& operator=(ActionSetConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_ACTION_SET_LOCALIZED_NAME_SIZE);
                BoolInputActionCount = src.BoolInputActionCount;
                BoolInputActions = src.BoolInputActions;
                FloatInputActionCount = src.FloatInputActionCount;
                FloatInputActions = src.FloatInputActions;
                Vec2InputActionCount = src.Vec2InputActionCount;
                Vec2InputActions = src.Vec2InputActions;
                PoseInputActionCount = src.PoseInputActionCount;
                PoseInputActions = src.PoseInputActions;
                HapticOutputActionCount = src.HapticOutputActionCount;
                HapticOutputActions = src.HapticOutputActions;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.BoolInputActionCount = 0;
                src.BoolInputActions = nullptr;
                src.FloatInputActionCount = 0;
                src.FloatInputActions = nullptr;
                src.Vec2InputActionCount = 0;
                src.Vec2InputActions = nullptr;
                src.PoseInputActionCount = 0;
                src.PoseInputActions = nullptr;
                src.HapticOutputActionCount = 0;
                src.HapticOutputActions = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ActionSetConfig as an AxrActionSetConfig
        /// @returns This as an AxrActionSetConfig
        const AxrActionSetConfig* toRaw() const {
            return reinterpret_cast<const AxrActionSetConfig*>(this);
        }

        /// Get a handle to the ActionSetConfig as an AxrActionSetConfig
        /// @returns This as an AxrActionSetConfig
        AxrActionSetConfig* toRaw() {
            return reinterpret_cast<AxrActionSetConfig*>(this);
        }

        /// Add the given bool input actions
        /// @param inputActions Bool input actions
        void addBoolInputActions(const std::vector<axr::BoolInputActionConfig>& inputActions) {
            addBoolInputActions(inputActions.size(), inputActions.data());
        }

        /// Add the given bool input actions
        /// @param inputActionCount Bool input actions count
        /// @param inputActions Bool input actions
        void addBoolInputActions(const uint32_t inputActionCount, const axr::BoolInputActionConfig* inputActions) {
            const uint32_t startingSize = BoolInputActionCount;
            resizeBoolInputActions(BoolInputActionCount + inputActionCount);

            for (uint32_t i = 0; i < inputActionCount; ++i) {
                BoolInputActions[startingSize + i] = axr::BoolInputActionConfig(
                    axrBoolInputActionConfigClone(inputActions[i].toRaw())
                );
            }
        }

        /// Add a bool input action
        /// @param inputAction Bool input action
        void addBoolInputAction(const axr::BoolInputActionConfig& inputAction) {
            resizeBoolInputActions(BoolInputActionCount + 1);

            BoolInputActions[BoolInputActionCount - 1] = axr::BoolInputActionConfig(
                axrBoolInputActionConfigClone(inputAction.toRaw())
            );
        }

        /// Add the given float input actions
        /// @param inputActions Float input actions
        void addFloatInputActions(const std::vector<axr::FloatInputActionConfig>& inputActions) {
            addFloatInputActions(inputActions.size(), inputActions.data());
        }

        /// Add the given float input actions
        /// @param inputActionCount Float input actions count
        /// @param inputActions Float input actions
        void addFloatInputActions(const uint32_t inputActionCount, const axr::FloatInputActionConfig* inputActions) {
            const uint32_t startingSize = FloatInputActionCount;
            resizeFloatInputActions(FloatInputActionCount + inputActionCount);

            for (uint32_t i = 0; i < inputActionCount; ++i) {
                FloatInputActions[startingSize + i] = axr::FloatInputActionConfig(
                    axrFloatInputActionConfigClone(inputActions[i].toRaw())
                );
            }
        }

        /// Add a float input action
        /// @param inputAction Float input action
        void addFloatInputAction(const axr::FloatInputActionConfig& inputAction) {
            resizeFloatInputActions(FloatInputActionCount + 1);

            FloatInputActions[FloatInputActionCount - 1] = axr::FloatInputActionConfig(
                axrFloatInputActionConfigClone(inputAction.toRaw())
            );
        }

        /// Add the given vec2 input actions
        /// @param inputActions Vec2 input actions
        void addVec2InputActions(const std::vector<axr::Vec2InputActionConfig>& inputActions) {
            addVec2InputActions(inputActions.size(), inputActions.data());
        }

        /// Add the given vec2 input actions
        /// @param inputActionCount Vec2 input actions count
        /// @param inputActions Vec2 input actions
        void addVec2InputActions(const uint32_t inputActionCount, const axr::Vec2InputActionConfig* inputActions) {
            const uint32_t startingSize = Vec2InputActionCount;
            resizeVec2InputActions(Vec2InputActionCount + inputActionCount);

            for (uint32_t i = 0; i < inputActionCount; ++i) {
                Vec2InputActions[startingSize + i] = axr::Vec2InputActionConfig(
                    axrVec2InputActionConfigClone(inputActions[i].toRaw())
                );
            }
        }

        /// Add a vec2 input action
        /// @param inputAction Vec2 input action
        void addVec2InputAction(const axr::Vec2InputActionConfig& inputAction) {
            resizeVec2InputActions(Vec2InputActionCount + 1);

            Vec2InputActions[Vec2InputActionCount - 1] = axr::Vec2InputActionConfig(
                axrVec2InputActionConfigClone(inputAction.toRaw())
            );
        }

        /// Add the given pose input actions
        /// @param inputActions Pose input actions
        void addPoseInputActions(const std::vector<axr::PoseInputActionConfig>& inputActions) {
            addPoseInputActions(inputActions.size(), inputActions.data());
        }

        /// Add the given pose input actions
        /// @param inputActionCount Pose input actions count
        /// @param inputActions Pose input actions
        void addPoseInputActions(const uint32_t inputActionCount, const axr::PoseInputActionConfig* inputActions) {
            const uint32_t startingSize = PoseInputActionCount;
            resizePoseInputActions(PoseInputActionCount + inputActionCount);

            for (uint32_t i = 0; i < inputActionCount; ++i) {
                PoseInputActions[startingSize + i] = axr::PoseInputActionConfig(
                    axrPoseInputActionConfigClone(inputActions[i].toRaw())
                );
            }
        }

        /// Add a pose input action
        /// @param inputAction Pose input action
        void addPoseInputAction(const axr::PoseInputActionConfig& inputAction) {
            resizePoseInputActions(PoseInputActionCount + 1);

            PoseInputActions[PoseInputActionCount - 1] = axr::PoseInputActionConfig(
                axrPoseInputActionConfigClone(inputAction.toRaw())
            );
        }

        /// Add the given haptic output actions
        /// @param outputActions Haptic output actions
        void addHapticOutputActions(const std::vector<axr::HapticOutputActionConfig>& outputActions) {
            addHapticOutputActions(outputActions.size(), outputActions.data());
        }

        /// Add the given haptic output actions
        /// @param outputActionCount Haptic output actions count
        /// @param outputActions Haptic output actions
        void addHapticOutputActions(
            const uint32_t outputActionCount,
            const axr::HapticOutputActionConfig* outputActions
        ) {
            const uint32_t startingSize = HapticOutputActionCount;
            resizeHapticOutputActions(HapticOutputActionCount + outputActionCount);

            for (uint32_t i = 0; i < outputActionCount; ++i) {
                HapticOutputActions[startingSize + i] = axr::HapticOutputActionConfig(
                    axrHapticOutputActionConfigClone(outputActions[i].toRaw())
                );
            }
        }

        /// Add a haptic output action
        /// @param outputAction Haptic output action
        void addHapticOutputAction(const axr::HapticOutputActionConfig& outputAction) {
            resizeHapticOutputActions(HapticOutputActionCount + 1);

            HapticOutputActions[HapticOutputActionCount - 1] = axr::HapticOutputActionConfig(
                axrHapticOutputActionConfigClone(outputAction.toRaw())
            );
        }

        /// Clear the bool input actions
        void clearBoolInputActions() {
            if (BoolInputActions == nullptr) return;

            for (uint32_t i = 0; i < BoolInputActionCount; ++i) {
                axrBoolInputActionConfigDestroy(BoolInputActions[i].toRaw());
            }

            delete[] BoolInputActions;
            BoolInputActions = nullptr;
            BoolInputActionCount = 0;
        }

        /// Clear the float input actions
        void clearFloatInputActions() {
            if (FloatInputActions == nullptr) return;

            for (uint32_t i = 0; i < FloatInputActionCount; ++i) {
                axrFloatInputActionConfigDestroy(FloatInputActions[i].toRaw());
            }

            delete[] FloatInputActions;
            FloatInputActions = nullptr;
            FloatInputActionCount = 0;
        }

        /// Clear the vec2 input actions
        void clearVec2InputActions() {
            if (Vec2InputActions == nullptr) return;

            for (uint32_t i = 0; i < Vec2InputActionCount; ++i) {
                axrVec2InputActionConfigDestroy(Vec2InputActions[i].toRaw());
            }

            delete[] Vec2InputActions;
            Vec2InputActions = nullptr;
            Vec2InputActionCount = 0;
        }

        /// Clear the pose input actions
        void clearPoseInputActions() {
            if (PoseInputActions == nullptr) return;

            for (uint32_t i = 0; i < PoseInputActionCount; ++i) {
                axrPoseInputActionConfigDestroy(PoseInputActions[i].toRaw());
            }

            delete[] PoseInputActions;
            PoseInputActions = nullptr;
            PoseInputActionCount = 0;
        }

        /// Clear the haptic output actions
        void clearHapticOutputActions() {
            if (HapticOutputActions == nullptr) return;

            for (uint32_t i = 0; i < HapticOutputActionCount; ++i) {
                axrHapticOutputActionConfigDestroy(HapticOutputActions[i].toRaw());
            }

            delete[] HapticOutputActions;
            HapticOutputActions = nullptr;
            HapticOutputActionCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            memset(Name, 0, sizeof(Name));
            memset(LocalizedName, 0, sizeof(LocalizedName));

            clearBoolInputActions();
            clearFloatInputActions();
            clearVec2InputActions();
            clearPoseInputActions();
            clearHapticOutputActions();
        }

        /// Resize the bool input actions
        /// @param size New size
        void resizeBoolInputActions(const uint32_t size) {
            if (size == 0) {
                clearBoolInputActions();
                return;
            }

            const auto newInputActions = new axr::BoolInputActionConfig[size]{};
            for (uint32_t i = 0; i < std::min(BoolInputActionCount, size); ++i) {
                newInputActions[i] = BoolInputActions[i];
                BoolInputActions[i] = {};
            }

            clearBoolInputActions();
            BoolInputActions = newInputActions;
            BoolInputActionCount = size;
        }

        /// Resize the float input actions
        /// @param size New size
        void resizeFloatInputActions(const uint32_t size) {
            if (size == 0) {
                clearFloatInputActions();
                return;
            }

            const auto newInputActions = new axr::FloatInputActionConfig[size]{};
            for (uint32_t i = 0; i < std::min(FloatInputActionCount, size); ++i) {
                newInputActions[i] = FloatInputActions[i];
                FloatInputActions[i] = {};
            }

            clearFloatInputActions();
            FloatInputActions = newInputActions;
            FloatInputActionCount = size;
        }

        /// Resize the vec2 input actions
        /// @param size New size
        void resizeVec2InputActions(const uint32_t size) {
            if (size == 0) {
                clearVec2InputActions();
                return;
            }

            const auto newInputActions = new axr::Vec2InputActionConfig[size]{};
            for (uint32_t i = 0; i < std::min(Vec2InputActionCount, size); ++i) {
                newInputActions[i] = Vec2InputActions[i];
                Vec2InputActions[i] = {};
            }

            clearVec2InputActions();
            Vec2InputActions = newInputActions;
            Vec2InputActionCount = size;
        }

        /// Resize the pose input actions
        /// @param size New size
        void resizePoseInputActions(const uint32_t size) {
            if (size == 0) {
                clearPoseInputActions();
                return;
            }

            const auto newInputActions = new axr::PoseInputActionConfig[size]{};
            for (uint32_t i = 0; i < std::min(PoseInputActionCount, size); ++i) {
                newInputActions[i] = PoseInputActions[i];
                PoseInputActions[i] = {};
            }

            clearPoseInputActions();
            PoseInputActions = newInputActions;
            PoseInputActionCount = size;
        }

        /// Resize the haptic output actions
        /// @param size New size
        void resizeHapticOutputActions(const uint32_t size) {
            if (size == 0) {
                clearHapticOutputActions();
                return;
            }

            const auto newInputActions = new axr::HapticOutputActionConfig[size]{};
            for (uint32_t i = 0; i < std::min(HapticOutputActionCount, size); ++i) {
                newInputActions[i] = HapticOutputActions[i];
                HapticOutputActions[i] = {};
            }

            clearHapticOutputActions();
            HapticOutputActions = newInputActions;
            HapticOutputActionCount = size;
        }
    };

    static_assert(
        sizeof(AxrActionSetConfig) == sizeof(axr::ActionSetConfig),
        "Original type and wrapper have different size!"
    );

    /// Action system config
    struct ActionSystemConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t ActionSetCount = 0;
        axr::ActionSetConfig* ActionSets = nullptr;
        uint32_t XrInteractionProfileCount = 0;
        axr::XrInteractionProfileEnum* XrInteractionProfiles = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ActionSystemConfig() = default;

        /// Constructor
        /// @param actionSets Action sets
        /// @param xrInteractionProfiles Xr interaction profiles
        ActionSystemConfig(
            const std::vector<axr::ActionSetConfig>& actionSets,
            const std::vector<axr::XrInteractionProfileEnum>& xrInteractionProfiles
        ) {
            addActionSets(actionSets);
            addXrInteractionProfiles(xrInteractionProfiles);
        }

        /// Copy Constructor
        /// @param src Source ActionSystemConfig to copy from
        ActionSystemConfig(const ActionSystemConfig& src) {
            addActionSets(src.ActionSetCount, src.ActionSets);
            addXrInteractionProfiles(src.XrInteractionProfileCount, src.XrInteractionProfiles);
        }

        /// Move Constructor
        /// @param src Source ActionSystemConfig to move from
        ActionSystemConfig(ActionSystemConfig&& src) noexcept {
            ActionSetCount = src.ActionSetCount;
            ActionSets = src.ActionSets;
            XrInteractionProfileCount = src.XrInteractionProfileCount;
            XrInteractionProfiles = src.XrInteractionProfiles;

            src.ActionSetCount = 0;
            src.ActionSets = nullptr;
            src.XrInteractionProfileCount = 0;
            src.XrInteractionProfiles = nullptr;
        }

        // ---- Destructor ----

        ~ActionSystemConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ActionSystemConfig to copy from
        ActionSystemConfig& operator=(const ActionSystemConfig& src) {
            if (this != &src) {
                cleanup();

                addActionSets(src.ActionSetCount, src.ActionSets);
                addXrInteractionProfiles(src.XrInteractionProfileCount, src.XrInteractionProfiles);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ActionSystemConfig to move from
        ActionSystemConfig& operator=(ActionSystemConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                ActionSetCount = src.ActionSetCount;
                ActionSets = src.ActionSets;
                XrInteractionProfileCount = src.XrInteractionProfileCount;
                XrInteractionProfiles = src.XrInteractionProfiles;

                src.ActionSetCount = 0;
                src.ActionSets = nullptr;
                src.XrInteractionProfileCount = 0;
                src.XrInteractionProfiles = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ActionSystemConfig as an AxrActionSystemConfig
        /// @returns This as an AxrActionSystemConfig
        const AxrActionSystemConfig* toRaw() const {
            return reinterpret_cast<const AxrActionSystemConfig*>(this);
        }

        /// Get a handle to the ActionSystemConfig as an AxrActionSystemConfig
        /// @returns This as an AxrActionSystemConfig
        AxrActionSystemConfig* toRaw() {
            return reinterpret_cast<AxrActionSystemConfig*>(this);
        }

        /// Add the given action sets
        /// @param actionSets Action sets
        void addActionSets(const std::vector<axr::ActionSetConfig>& actionSets) {
            addActionSets(actionSets.size(), actionSets.data());
        }

        /// Add the given action sets
        /// @param actionSetCount Action sets count
        /// @param actionSets Action sets
        void addActionSets(const uint32_t actionSetCount, const axr::ActionSetConfig* actionSets) {
            const uint32_t startingSize = ActionSetCount;
            resizeActionSets(ActionSetCount + actionSetCount);

            for (uint32_t i = 0; i < actionSetCount; ++i) {
                ActionSets[startingSize + i] = axr::ActionSetConfig(
                    axrActionSetConfigClone(actionSets[i].toRaw())
                );
            }
        }

        /// Add an action set
        /// @param actionSet Action set
        void addActionSet(const axr::ActionSetConfig& actionSet) {
            resizeActionSets(ActionSetCount + 1);

            ActionSets[ActionSetCount - 1] = axr::ActionSetConfig(
                axrActionSetConfigClone(actionSet.toRaw())
            );
        }

        /// Add the given xr interaction profiles
        /// @param xrInteractionProfiles Xr interaction profiles
        void addXrInteractionProfiles(const std::vector<axr::XrInteractionProfileEnum>& xrInteractionProfiles) {
            addXrInteractionProfiles(
                xrInteractionProfiles.size(),
                xrInteractionProfiles.data()
            );
        }

        /// Add the given xr interaction profiles
        /// @param xrInteractionProfileCount Xr interaction profile count
        /// @param xrInteractionProfiles Xr interaction profiles
        void addXrInteractionProfiles(
            const uint32_t xrInteractionProfileCount,
            const axr::XrInteractionProfileEnum* xrInteractionProfiles
        ) {
            const uint32_t startingSize = XrInteractionProfileCount;
            resizeXrInteractionProfiles(XrInteractionProfileCount + xrInteractionProfileCount);

            for (uint32_t i = 0; i < xrInteractionProfileCount; ++i) {
                XrInteractionProfiles[startingSize + i] = xrInteractionProfiles[i];
            }
        }

        /// Add an xr interaction profile
        /// @param xrInteractionProfile Xr interaction profile
        void addXrInteractionProfile(const axr::XrInteractionProfileEnum xrInteractionProfile) {
            resizeXrInteractionProfiles(XrInteractionProfileCount + 1);

            XrInteractionProfiles[XrInteractionProfileCount - 1] = xrInteractionProfile;
        }

        /// Clear the action sets
        void clearActionSets() {
            if (ActionSets == nullptr) return;

            for (uint32_t i = 0; i < ActionSetCount; ++i) {
                axrActionSetConfigDestroy(ActionSets[i].toRaw());
            }

            delete[] ActionSets;
            ActionSets = nullptr;
            ActionSetCount = 0;
        }

        /// Clear the xr interaction profiles
        void clearXrInteractionProfiles() {
            if (XrInteractionProfiles == nullptr) return;

            delete[] XrInteractionProfiles;
            XrInteractionProfiles = nullptr;
            XrInteractionProfileCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            clearActionSets();
            clearXrInteractionProfiles();
        }

        /// Resize the action sets
        /// @param size New size
        void resizeActionSets(const uint32_t size) {
            if (size == 0) {
                clearActionSets();
                return;
            }

            const auto newActionSets = new axr::ActionSetConfig[size]{};
            for (uint32_t i = 0; i < std::min(ActionSetCount, size); ++i) {
                newActionSets[i] = ActionSets[i];
                ActionSets[i] = {};
            }

            clearActionSets();
            ActionSets = newActionSets;
            ActionSetCount = size;
        }

        /// Resize the xr interaction profiles
        /// @param size New size
        void resizeXrInteractionProfiles(const uint32_t size) {
            if (size == 0) {
                clearXrInteractionProfiles();
                return;
            }

            const auto newXrInteractionProfiles = new axr::XrInteractionProfileEnum[size]{};
            for (uint32_t i = 0; i < std::min(XrInteractionProfileCount, size); ++i) {
                newXrInteractionProfiles[i] = XrInteractionProfiles[i];
                XrInteractionProfiles[i] = {};
            }

            clearXrInteractionProfiles();
            XrInteractionProfiles = newXrInteractionProfiles;
            XrInteractionProfileCount = size;
        }
    };

    static_assert(
        sizeof(AxrActionSystemConfig) == sizeof(axr::ActionSystemConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Bool Input Action Definition
    // ----------------------------------------- //

    /// Bool Input Action
    class BoolInputAction {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param boolInputAction Bool input action handle
        explicit BoolInputAction(const AxrBoolInputAction_T boolInputAction):
            m_BoolInputAction(boolInputAction) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Enable the bool action set
        void enable() const {
            axrBoolInputActionEnable(m_BoolInputAction);
        }

        /// Disable the bool action set
        void disable() const {
            axrBoolInputActionDisable(m_BoolInputAction);
        }

        /// Check if the action is enabled
        /// @returns True if the action is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrBoolInputActionIsEnabled(m_BoolInputAction);
        }

        /// Check if the value has changed since the last frame
        /// @returns True if the value has changed since the last frame
        [[nodiscard]] bool valueChanged() const {
            return axrBoolInputActionValueChanged(m_BoolInputAction);
        }

        /// Get the current value of this input action
        /// @returns The current value of this input action
        [[nodiscard]] bool getValue() const {
            return axrBoolInputActionGetValue(m_BoolInputAction);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrBoolInputAction_T m_BoolInputAction;
    };

    // ----------------------------------------- //
    // Float Input Action Definition
    // ----------------------------------------- //

    /// Float Input Action
    class FloatInputAction {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param floatInputAction Float input action handle
        explicit FloatInputAction(const AxrFloatInputAction_T floatInputAction):
            m_FloatInputAction(floatInputAction) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Enable the float action set
        void enable() const {
            axrFloatInputActionEnable(m_FloatInputAction);
        }

        /// Disable the float action set
        void disable() const {
            axrFloatInputActionDisable(m_FloatInputAction);
        }

        /// Check if the action is enabled
        /// @returns True if the action is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrFloatInputActionIsEnabled(m_FloatInputAction);
        }

        /// Check if the value has changed since the last frame
        /// @returns True if the value has changed since the last frame
        [[nodiscard]] bool valueChanged() const {
            return axrFloatInputActionValueChanged(m_FloatInputAction);
        }

        /// Get the current value of this input action
        /// @returns The current value of this input action
        [[nodiscard]] float getValue() const {
            return axrFloatInputActionGetValue(m_FloatInputAction);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrFloatInputAction_T m_FloatInputAction;
    };

    // ----------------------------------------- //
    // Vec2 Input Action Definition
    // ----------------------------------------- //

    /// Vec2 Input Action
    class Vec2InputAction {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param vec2InputAction Vec2 input action handle
        explicit Vec2InputAction(const AxrVec2InputAction_T vec2InputAction):
            m_Vec2InputAction(vec2InputAction) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Enable the vec2 action set
        void enable() const {
            axrVec2InputActionEnable(m_Vec2InputAction);
        }

        /// Disable the vec2 action set
        void disable() const {
            axrVec2InputActionDisable(m_Vec2InputAction);
        }

        /// Check if the action is enabled
        /// @returns True if the action is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrVec2InputActionIsEnabled(m_Vec2InputAction);
        }

        /// Check if the value has changed since the last frame
        /// @returns True if the value has changed since the last frame
        [[nodiscard]] bool valueChanged() const {
            return axrVec2InputActionValueChanged(m_Vec2InputAction);
        }

        /// Get the current value of this input action
        /// @returns The current value of this input action
        [[nodiscard]] axr::Vec2 getValue() const {
            return axr::Vec2(axrVec2InputActionGetValue(m_Vec2InputAction));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrVec2InputAction_T m_Vec2InputAction;
    };

    // ----------------------------------------- //
    // Pose Input Action Definition
    // ----------------------------------------- //

    /// Pose Input Action
    class PoseInputAction {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param poseInputAction Pose input action handle
        explicit PoseInputAction(const AxrPoseInputAction_T poseInputAction):
            m_PoseInputAction(poseInputAction) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Enable the pose action set
        void enable() const {
            axrPoseInputActionEnable(m_PoseInputAction);
        }

        /// Disable the pose action set
        void disable() const {
            axrPoseInputActionDisable(m_PoseInputAction);
        }

        /// Check if the action is enabled
        /// @returns True if the action is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrPoseInputActionIsEnabled(m_PoseInputAction);
        }

        /// Get the current value of this input action
        /// @returns The current value of this input action
        [[nodiscard]] axr::Pose getValue() const {
            return axr::Pose(axrPoseInputActionGetValue(m_PoseInputAction));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrPoseInputAction_T m_PoseInputAction;
    };

    // ----------------------------------------- //
    // Haptic Output Action Definition
    // ----------------------------------------- //

    /// Haptic Output Action
    class HapticOutputAction {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param hapticOutputAction Haptic output action handle
        explicit HapticOutputAction(const AxrHapticOutputAction_T hapticOutputAction):
            m_HapticOutputAction(hapticOutputAction) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Enable the haptic output set
        void enable() const {
            axrHapticOutputActionEnable(m_HapticOutputAction);
        }

        /// Disable the haptic output set
        void disable() const {
            axrHapticOutputActionDisable(m_HapticOutputAction);
        }

        /// Check if the action is enabled
        /// @returns True if the action is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrHapticOutputActionIsEnabled(m_HapticOutputAction);
        }

        /// Activate the action haptics
        /// @param duration Haptic duration in nanoseconds
        /// @param frequency Haptic frequency in Hz
        /// @param amplitude Haptic amplitude from 0.0f to 1.0f
        void activate(
            const int64_t duration,
            const float frequency,
            const float amplitude
        ) const {
            axrHapticOutputActionActivate(m_HapticOutputAction, duration, frequency, amplitude);
        }

        /// Deactivate the action haptics
        void deactivate() const {
            axrHapticOutputActionDeactivate(m_HapticOutputAction);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrHapticOutputAction_T m_HapticOutputAction;
    };

    // ----------------------------------------- //
    // Action Set Definition
    // ----------------------------------------- //

    /// Action Set
    class ActionSet {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param actionSet Action set handle
        explicit ActionSet(const AxrActionSet_T actionSet):
            m_ActionSet(actionSet) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Set the priority over other action sets. Higher number = Higher priority.
        /// @param priority new priority
        void setPriority(const uint32_t priority) const {
            axrActionSetSetPriority(m_ActionSet, priority);
        }

        /// Get the priority level
        /// @returns The priority level
        [[nodiscard]] uint32_t getPriority() const {
            return axrActionSetGetPriority(m_ActionSet);
        }

        /// Enable the action set
        void enable() const {
            axrActionSetEnable(m_ActionSet);
        }

        /// Disable the action set
        void disable() const {
            axrActionSetDisable(m_ActionSet);
        }

        /// Check if the action set is enabled
        /// @returns True if the action set is enabled
        [[nodiscard]] bool isEnabled() const {
            return axrActionSetIsEnabled(m_ActionSet);
        }

        /// Get the named bool input action
        /// @param name Bool input action name
        /// @returns The bool input action or nullptr if it wasn't found
        [[nodiscard]] axr::BoolInputAction getBoolInputAction(const char* name) const {
            return axr::BoolInputAction(axrActionSetGetBoolInputAction(m_ActionSet, name));
        }

        /// Get the named float input action
        /// @param name Float input action name
        /// @returns The float input action or nullptr if it wasn't found
        [[nodiscard]] axr::FloatInputAction getFloatInputAction(const char* name) const {
            return axr::FloatInputAction(axrActionSetGetFloatInputAction(m_ActionSet, name));
        }

        /// Get the named vec2 input action
        /// @param name Vec2 input action name
        /// @returns The vec2 input action or nullptr if it wasn't found
        [[nodiscard]] axr::Vec2InputAction getVec2InputAction(const char* name) const {
            return axr::Vec2InputAction(axrActionSetGetVec2InputAction(m_ActionSet, name));
        }

        /// Get the named pose input action
        /// @param name Pose input action name
        /// @returns The pose input action or nullptr if it wasn't found
        [[nodiscard]] axr::PoseInputAction getPoseInputAction(const char* name) const {
            return axr::PoseInputAction(axrActionSetGetPoseInputAction(m_ActionSet, name));
        }

        /// Get the named haptic output action
        /// @param name Haptic output action name
        /// @returns The haptic output action or nullptr if it wasn't found
        [[nodiscard]] axr::HapticOutputAction getHapticOutputAction(const char* name) const {
            return axr::HapticOutputAction(axrActionSetGetHapticOutputAction(m_ActionSet, name));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrActionSet_T m_ActionSet;
    };

    // ----------------------------------------- //
    // Action System Definition
    // ----------------------------------------- //

    /// Action System
    class ActionSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param actionSystem Action system handle
        explicit ActionSystem(const AxrActionSystem_T actionSystem):
            m_ActionSystem(actionSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the named action set
        /// @param name Action set name
        /// @returns The action set or nullptr if it wasn't found
        axr::ActionSet getActionSet(const char* name) const {
            return axr::ActionSet(axrActionSystemGetActionSet(m_ActionSystem, name));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrActionSystem_T m_ActionSystem;
    };
}
