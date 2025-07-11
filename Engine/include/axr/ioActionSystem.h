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
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>

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

    // ---- Mouse - Max of 4 ----
    AXR_FLOAT_INPUT_ACTION_MOUSE_START = 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL = 1,
    AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL = 2,
    AXR_FLOAT_INPUT_ACTION_MOUSE_END = 4,
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

/// Input/Output action system config
struct AxrIOActionSystemConfig {
    uint32_t IOActionSetCount;
    AxrIOActionSetConfig* IOActionSets;
    // TODO: Add OpenXR interaction profiles here too
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

    /// Check if the value was set this frame
    /// @param inputAction Bool input action to use
    /// @returns True if the value was set this frame
    AXR_API bool axrBoolInputActionWasValueSetThisFrame(AxrBoolInputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Bool input action to use
    /// @returns The current value of this input action
    AXR_API bool axrBoolInputActionGetValue(AxrBoolInputActionConst_T inputAction);

    // ---- Float Input Action ----

    /// Check if the value was set this frame
    /// @param inputAction Float input action to use
    /// @returns True if the value was set this frame
    AXR_API bool axrFloatInputActionWasValueSetThisFrame(AxrFloatInputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Float input action to use
    /// @returns The current value of this input action
    AXR_API float axrFloatInputActionGetValue(AxrFloatInputActionConst_T inputAction);

    // ---- Vec2 Input Action ----

    /// Check if the value was set this frame
    /// @param inputAction Vec2 input action to use
    /// @returns True if the value was set this frame
    AXR_API bool axrVec2InputActionWasValueSetThisFrame(AxrVec2InputActionConst_T inputAction);
    /// Get the current value of this input action
    /// @param inputAction Vec2 input action to use
    /// @returns The current value of this input action
    // TODO: Fix the C linkage warning here.
    //  warning C4190: 'axrVec2InputActionGetValue' has C-linkage specified, but returns 'glm::vec<2,float,glm::packed_highp>' which is incompatible with C
    AXR_API glm::vec2 axrVec2InputActionGetValue(AxrVec2InputActionConst_T inputAction);

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
