#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionSystem.h"
#include "common/types.hpp"

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
    };

    /// Float input action enum
    enum class FloatInputActionEnum {
        Undefined = AXR_FLOAT_INPUT_ACTION_UNDEFINED,

        // ---- Mouse - Max of 4 ----
        MouseStart = AXR_FLOAT_INPUT_ACTION_MOUSE_START,
        MouseWheel = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL,
        MouseWheelHorizontal = AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL,
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
    // Structs
    // ----------------------------------------- //

    /// Bool input action config
    struct BoolInputActionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_IO_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE]{};
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
        BoolInputActionConfig(
            const char* name,
            const char* localizedName
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param bindings Bool bindings
        BoolInputActionConfig(
            const char* name,
            const char* localizedName,
            const std::vector<axr::BoolInputActionEnum>& bindings
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source BoolInputActionConfig
        explicit BoolInputActionConfig(AxrBoolInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::BoolInputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source BoolInputActionConfig to copy from
        BoolInputActionConfig(const BoolInputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source BoolInputActionConfig to move from
        BoolInputActionConfig(BoolInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source BoolInputActionConfig to move from
        BoolInputActionConfig& operator=(BoolInputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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
            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
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
        char Name[AXR_MAX_IO_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE]{};
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
        FloatInputActionConfig(
            const char* name,
            const char* localizedName
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param bindings Float bindings
        FloatInputActionConfig(
            const char* name,
            const char* localizedName,
            const std::vector<axr::FloatInputActionEnum>& bindings
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source FloatInputActionConfig
        explicit FloatInputActionConfig(AxrFloatInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::FloatInputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source FloatInputActionConfig to copy from
        FloatInputActionConfig(const FloatInputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source FloatInputActionConfig to move from
        FloatInputActionConfig(FloatInputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source FloatInputActionConfig to move from
        FloatInputActionConfig& operator=(FloatInputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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

            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
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
        char Name[AXR_MAX_IO_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE]{};
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
        Vec2InputActionConfig(
            const char* name,
            const char* localizedName
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param bindings Vec2 bindings
        Vec2InputActionConfig(
            const char* name,
            const char* localizedName,
            const std::vector<axr::Vec2InputActionEnum>& bindings
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            }

            addBindings(bindings);
        }

        /// Constructor
        /// @param src Source Vec2InputActionConfig
        explicit Vec2InputActionConfig(AxrVec2InputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = reinterpret_cast<axr::Vec2InputActionEnum*>(src.Bindings);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BindingCount = 0;
            src.Bindings = nullptr;
        }

        /// Copy Constructor
        /// @param src Source Vec2InputActionConfig to copy from
        Vec2InputActionConfig(const Vec2InputActionConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            addBindings(src.BindingCount, src.Bindings);
        }

        /// Move Constructor
        /// @param src Source Vec2InputActionConfig to move from
        Vec2InputActionConfig(Vec2InputActionConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
            BindingCount = src.BindingCount;
            Bindings = src.Bindings;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                addBindings(src.BindingCount, src.Bindings);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Vec2InputActionConfig to move from
        Vec2InputActionConfig& operator=(Vec2InputActionConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE);
                BindingCount = src.BindingCount;
                Bindings = src.Bindings;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
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

            clearBindings();
        }

        /// Resize the bindings
        /// @param size New size
        void resizeBindings(const uint32_t size) {
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

    /// Input/Output action set config
    struct IOActionSetConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char Name[AXR_MAX_IO_ACTION_NAME_SIZE]{};
        char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE]{};
        uint32_t BoolInputActionCount = 0;
        axr::BoolInputActionConfig* BoolInputActions = nullptr;
        uint32_t FloatInputActionCount = 0;
        axr::FloatInputActionConfig* FloatInputActions = nullptr;
        uint32_t Vec2InputActionCount = 0;
        axr::Vec2InputActionConfig* Vec2InputActions = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        IOActionSetConfig() = default;

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        IOActionSetConfig(
            const char* name,
            const char* localizedName
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
            }
        }

        /// Constructor
        /// @param name Input action name
        /// @param localizedName Input action localized name
        /// @param boolInputActions Bool input actions
        /// @param floatInputActions Float input actions
        /// @param vec2InputActions Vec2 input actions
        IOActionSetConfig(
            const char* name,
            const char* localizedName,
            const std::vector<axr::BoolInputActionConfig>& boolInputActions,
            const std::vector<axr::FloatInputActionConfig>& floatInputActions,
            const std::vector<axr::Vec2InputActionConfig>& vec2InputActions
        ) {
            if (name != nullptr) {
                strncpy_s(Name, name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
            }
            if (localizedName != nullptr) {
                strncpy_s(LocalizedName, localizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
            }

            addBoolInputActions(boolInputActions);
            addFloatInputActions(floatInputActions);
            addVec2InputActions(vec2InputActions);
        }

        /// Constructor
        /// @param src Source IOActionSetConfig
        explicit IOActionSetConfig(AxrIOActionSetConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
            BoolInputActionCount = src.BoolInputActionCount;
            BoolInputActions = reinterpret_cast<axr::BoolInputActionConfig*>(src.BoolInputActions);
            FloatInputActionCount = src.FloatInputActionCount;
            FloatInputActions = reinterpret_cast<axr::FloatInputActionConfig*>(src.FloatInputActions);
            Vec2InputActionCount = src.Vec2InputActionCount;
            Vec2InputActions = reinterpret_cast<axr::Vec2InputActionConfig*>(src.Vec2InputActions);

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BoolInputActionCount = 0;
            src.BoolInputActions = nullptr;
            src.FloatInputActionCount = 0;
            src.FloatInputActions = nullptr;
            src.Vec2InputActionCount = 0;
            src.Vec2InputActions = nullptr;
        }

        /// Copy Constructor
        /// @param src Source IOActionSetConfig to copy from
        IOActionSetConfig(const IOActionSetConfig& src) {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
            addBoolInputActions(src.BoolInputActionCount, src.BoolInputActions);
            addFloatInputActions(src.FloatInputActionCount, src.FloatInputActions);
            addVec2InputActions(src.Vec2InputActionCount, src.Vec2InputActions);
        }

        /// Move Constructor
        /// @param src Source IOActionSetConfig to move from
        IOActionSetConfig(IOActionSetConfig&& src) noexcept {
            strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
            strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
            BoolInputActionCount = src.BoolInputActionCount;
            BoolInputActions = src.BoolInputActions;
            FloatInputActionCount = src.FloatInputActionCount;
            FloatInputActions = src.FloatInputActions;
            Vec2InputActionCount = src.Vec2InputActionCount;
            Vec2InputActions = src.Vec2InputActions;

            memset(src.Name, 0, sizeof(src.Name));
            memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
            src.BoolInputActionCount = 0;
            src.BoolInputActions = nullptr;
            src.FloatInputActionCount = 0;
            src.FloatInputActions = nullptr;
            src.Vec2InputActionCount = 0;
            src.Vec2InputActions = nullptr;
        }

        // ---- Destructor ----

        ~IOActionSetConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source IOActionSetConfig to copy from
        IOActionSetConfig& operator=(const IOActionSetConfig& src) {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
                addBoolInputActions(src.BoolInputActionCount, src.BoolInputActions);
                addFloatInputActions(src.FloatInputActionCount, src.FloatInputActions);
                addVec2InputActions(src.Vec2InputActionCount, src.Vec2InputActions);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source IOActionSetConfig to move from
        IOActionSetConfig& operator=(IOActionSetConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                strncpy_s(Name, src.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
                strncpy_s(LocalizedName, src.LocalizedName, AXR_MAX_IO_ACTION_SET_LOCALIZED_NAME_SIZE);
                BoolInputActionCount = src.BoolInputActionCount;
                BoolInputActions = src.BoolInputActions;
                FloatInputActionCount = src.FloatInputActionCount;
                FloatInputActions = src.FloatInputActions;
                Vec2InputActionCount = src.Vec2InputActionCount;
                Vec2InputActions = src.Vec2InputActions;

                memset(src.Name, 0, sizeof(src.Name));
                memset(src.LocalizedName, 0, sizeof(src.LocalizedName));
                src.BoolInputActionCount = 0;
                src.BoolInputActions = nullptr;
                src.FloatInputActionCount = 0;
                src.FloatInputActions = nullptr;
                src.Vec2InputActionCount = 0;
                src.Vec2InputActions = nullptr;
            }
            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the IOActionSetConfig as an AxrIOActionSetConfig
        /// @returns This as an AxrIOActionSetConfig
        const AxrIOActionSetConfig* toRaw() const {
            return reinterpret_cast<const AxrIOActionSetConfig*>(this);
        }

        /// Get a handle to the IOActionSetConfig as an AxrIOActionSetConfig
        /// @returns This as an AxrIOActionSetConfig
        AxrIOActionSetConfig* toRaw() {
            return reinterpret_cast<AxrIOActionSetConfig*>(this);
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
        }

        /// Resize the bool input actions
        /// @param size New size
        void resizeBoolInputActions(const uint32_t size) {
            // TODO: Add this size check for any other hpp struct that resizes in this way
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
    };

    static_assert(
        sizeof(AxrIOActionSetConfig) == sizeof(axr::IOActionSetConfig),
        "Original type and wrapper have different size!"
    );

    /// Input/Output action system config
    struct IOActionSystemConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t IOActionSetCount = 0;
        axr::IOActionSetConfig* IOActionSets = nullptr;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        IOActionSystemConfig() = default;

        /// Constructor
        /// @param ioActionSets Input/Output action sets
        IOActionSystemConfig(
            const std::vector<axr::IOActionSetConfig>& ioActionSets
        ) {
            addIOActionSets(ioActionSets);
        }

        /// Copy Constructor
        /// @param src Source IOActionSystemConfig to copy from
        IOActionSystemConfig(const IOActionSystemConfig& src) {
            addIOActionSets(src.IOActionSetCount, src.IOActionSets);
        }

        /// Move Constructor
        /// @param src Source IOActionSystemConfig to move from
        IOActionSystemConfig(IOActionSystemConfig&& src) noexcept {
            IOActionSetCount = src.IOActionSetCount;
            IOActionSets = src.IOActionSets;

            src.IOActionSetCount = 0;
            src.IOActionSets = nullptr;
        }

        // ---- Destructor ----

        ~IOActionSystemConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source IOActionSystemConfig to copy from
        IOActionSystemConfig& operator=(const IOActionSystemConfig& src) {
            if (this != &src) {
                cleanup();

                addIOActionSets(src.IOActionSetCount, src.IOActionSets);
            }
            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source IOActionSystemConfig to move from
        IOActionSystemConfig& operator=(IOActionSystemConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                IOActionSetCount = src.IOActionSetCount;
                IOActionSets = src.IOActionSets;

                src.IOActionSetCount = 0;
                src.IOActionSets = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the IOActionSystemConfig as an AxrIOActionSystemConfig
        /// @returns This as an AxrIOActionSystemConfig
        const AxrIOActionSystemConfig* toRaw() const {
            return reinterpret_cast<const AxrIOActionSystemConfig*>(this);
        }

        /// Get a handle to the IOActionSystemConfig as an AxrIOActionSystemConfig
        /// @returns This as an AxrIOActionSystemConfig
        AxrIOActionSystemConfig* toRaw() {
            return reinterpret_cast<AxrIOActionSystemConfig*>(this);
        }

        /// Add the given input/output action sets
        /// @param ioActionSets Input/Output action sets
        void addIOActionSets(const std::vector<axr::IOActionSetConfig>& ioActionSets) {
            addIOActionSets(ioActionSets.size(), ioActionSets.data());
        }

        /// Add the given input/output action sets
        /// @param ioActionSetCount Input/Output action sets count
        /// @param ioActionSets Input/Output action sets
        void addIOActionSets(const uint32_t ioActionSetCount, const axr::IOActionSetConfig* ioActionSets) {
            const uint32_t startingSize = IOActionSetCount;
            resizeIOActionSets(IOActionSetCount + ioActionSetCount);

            for (uint32_t i = 0; i < ioActionSetCount; ++i) {
                IOActionSets[startingSize + i] = axr::IOActionSetConfig(
                    axrIOActionSetConfigClone(ioActionSets[i].toRaw())
                );
            }
        }

        /// Add an input/output action set
        /// @param ioActionSet Input/Output action set
        void addIOActionSet(const axr::IOActionSetConfig& ioActionSet) {
            resizeIOActionSets(IOActionSetCount + 1);

            IOActionSets[IOActionSetCount - 1] = axr::IOActionSetConfig(
                axrIOActionSetConfigClone(ioActionSet.toRaw())
            );
        }

        /// Clear the input/output action sets
        void clearIOActionSets() {
            if (IOActionSets == nullptr) return;

            for (uint32_t i = 0; i < IOActionSetCount; ++i) {
                axrIOActionSetConfigDestroy(IOActionSets[i].toRaw());
            }

            delete[] IOActionSets;
            IOActionSets = nullptr;
            IOActionSetCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            clearIOActionSets();
        }

        /// Resize the input/output action set
        /// @param size New size
        void resizeIOActionSets(const uint32_t size) {
            const auto newIOActionSets = new axr::IOActionSetConfig[size]{};
            for (uint32_t i = 0; i < std::min(IOActionSetCount, size); ++i) {
                newIOActionSets[i] = IOActionSets[i];
                IOActionSets[i] = {};
            }

            clearIOActionSets();
            IOActionSets = newIOActionSets;
            IOActionSetCount = size;
        }
    };

    static_assert(
        sizeof(AxrIOActionSystemConfig) == sizeof(axr::IOActionSystemConfig),
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

        /// Check if the value was set this frame
        /// @returns True if the value was set this frame
        [[nodiscard]] bool wasValueSetThisFrame() const {
            return axrBoolInputActionWasValueSetThisFrame(m_BoolInputAction);
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

        /// Check if the value was set this frame
        /// @returns True if the value was set this frame
        [[nodiscard]] bool wasValueSetThisFrame() const {
            return axrFloatInputActionWasValueSetThisFrame(m_FloatInputAction);
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

        /// Check if the value was set this frame
        /// @returns True if the value was set this frame
        [[nodiscard]] bool wasValueSetThisFrame() const {
            return axrVec2InputActionWasValueSetThisFrame(m_Vec2InputAction);
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
    // Input/Output Action Set Definition
    // ----------------------------------------- //

    /// Input/Output Action Set
    class IOActionSet {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param iOActionSet Input/Output action set handle
        explicit IOActionSet(const AxrIOActionSet_T iOActionSet):
            m_IOActionSet(iOActionSet) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the named bool input action
        /// @param name Bool input action name
        /// @returns The bool input action or nullptr if it wasn't found
        [[nodiscard]] axr::BoolInputAction getBoolInputAction(const char* name) const {
            return axr::BoolInputAction(axrIOActionSetGetBoolInputAction(m_IOActionSet, name));
        }

        /// Get the named float input action
        /// @param name Float input action name
        /// @returns The float input action or nullptr if it wasn't found
        [[nodiscard]] axr::FloatInputAction getFloatInputAction(const char* name) const {
            return axr::FloatInputAction(axrIOActionSetGetFloatInputAction(m_IOActionSet, name));
        }

        /// Get the named vec2 input action
        /// @param name Vec2 input action name
        /// @returns The vec2 input action or nullptr if it wasn't found
        [[nodiscard]] axr::Vec2InputAction getVec2InputAction(const char* name) const {
            return axr::Vec2InputAction(axrIOActionSetGetVec2InputAction(m_IOActionSet, name));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrIOActionSet_T m_IOActionSet;
    };

    // ----------------------------------------- //
    // Input/Output Action System Definition
    // ----------------------------------------- //

    /// Input/Output Action System
    class IOActionSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param ioActionSystem Input/Output action system handle
        explicit IOActionSystem(const AxrIOActionSystem_T ioActionSystem):
            m_IOActionSystem(ioActionSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the named input/output action set
        /// @param name Input/Output action set name
        /// @returns The input/output action set or nullptr if it wasn't found
        axr::IOActionSet getIOActionSet(const char* name) const {
            return axr::IOActionSet(axrIOActionSystemGetIOActionSet(m_IOActionSystem, name));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrIOActionSystem_T m_IOActionSystem;
    };
}
