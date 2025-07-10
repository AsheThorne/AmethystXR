#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>

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
// Structs
// ----------------------------------------- //

/// Bool Input Action Config
struct AxrBoolInputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
    uint32_t BindingCount;
    AxrBoolInputActionEnum* Bindings;
};

/// Float Input Action Config
struct AxrFloatInputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
    uint32_t BindingCount;
    AxrFloatInputActionEnum* Bindings;
};

/// Vec2 Input Action Config
struct AxrVec2InputActionConfig {
    char Name[AXR_MAX_IO_ACTION_NAME_SIZE];
    char LocalizedName[AXR_MAX_IO_ACTION_LOCALIZED_NAME_SIZE];
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

/// Input/Output actions system config
struct AxrIOActionsSystemConfig {
    uint32_t IOActionSetCount;
    AxrIOActionSetConfig* IOActionSets;
    // TODO: Add OpenXR interaction profiles here too
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrIOActionsSystem Handle
typedef class AxrIOActionsSystem* AxrIOActionsSystem_T;
/// const AxrIOActionsSystem Handle
typedef const AxrIOActionsSystem* AxrIOActionsSystemConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
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
    AXR_API AxrFloatInputActionConfig axrFloatInputActionConfigClone(const AxrFloatInputActionConfig* inputActionConfig);
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
}
