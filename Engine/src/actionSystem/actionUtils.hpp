#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

#ifdef AXR_USE_PLATFORM_WIN32
/// Convert the given wParam to an AxrBoolInputActionEnum
/// @param wParam wParam to convert
/// @returns the converted wParam
[[nodiscard]] AxrBoolInputActionEnum axrWParamToBoolInputActionEnum(WPARAM wParam);
#endif

/// Check if the given bool input action is an xr bool input action
/// @param inputAction Bool input action
/// @returns True if the given bool input action is an xr bool input action
[[nodiscard]] bool axrIsXrBoolInputAction(AxrBoolInputActionEnum inputAction);
/// Check if the given float input action is an xr float input action
/// @param inputAction Float input action
/// @returns True if the given float input action is an xr float input action
[[nodiscard]] bool axrIsXrFloatInputAction(AxrFloatInputActionEnum inputAction);
/// Check if the given vec2 input action is an xr vec2 input action
/// @param inputAction Vec2 input action
/// @returns True if the given vec2 input action is an xr vec2 input action
[[nodiscard]] bool axrIsXrVec2InputAction(AxrVec2InputActionEnum inputAction);
/// Check if the given pose input action is an xr pose input action
/// @param inputAction Pose input action
/// @returns True if the given pose input action is an xr pose input action
[[nodiscard]] bool axrIsXrPoseInputAction(AxrPoseInputActionEnum inputAction);
/// Check if the given haptic output action is an xr haptic output action
/// @param outputAction Haptic output action
/// @returns True if the given haptic output action is an xr haptic output action
[[nodiscard]] bool axrIsXrHapticOutputAction(AxrHapticOutputActionEnum outputAction);

/// Get the xr path name for the given bool input action
/// @param inputAction Input action
/// @returns The action xr path name
[[nodiscard]] const char* axrGetXrBoolInputActionName(AxrBoolInputActionEnum inputAction);
/// Get the xr path name for the given float input action
/// @param inputAction Input action
/// @returns The action xr path name
[[nodiscard]] const char* axrGetXrFloatInputActionName(AxrFloatInputActionEnum inputAction);
/// Get the xr path name for the given vec2 input action
/// @param inputAction Input action
/// @returns The action xr path name
[[nodiscard]] const char* axrGetXrVec2InputActionName(AxrVec2InputActionEnum inputAction);
/// Get the xr path name for the given pose input action
/// @param inputAction Input action
/// @returns The action xr path name
[[nodiscard]] const char* axrGetXrPoseInputActionName(AxrPoseInputActionEnum inputAction);
/// Get the xr path name for the given haptic output action
/// @param outputAction Output action
/// @returns The action xr path name
[[nodiscard]] const char* axrGetXrHapticOutputActionName(AxrHapticOutputActionEnum outputAction);

/// Get all the bool input actions for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The collection of bool input actions
[[nodiscard]] std::unordered_set<AxrBoolInputActionEnum> axrGetXrInteractionProfileBoolInputActionBindings(
    AxrXrInteractionProfileEnum xrInteractionProfile
);
/// Get all the float input actions for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The collection of float input actions
[[nodiscard]] std::unordered_set<AxrFloatInputActionEnum> axrGetXrInteractionProfileFloatInputActionBindings(
    AxrXrInteractionProfileEnum xrInteractionProfile
);
/// Get all the vec2 input actions for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The collection of vec2 input actions
[[nodiscard]] std::unordered_set<AxrVec2InputActionEnum> axrGetXrInteractionProfileVec2InputActionBindings(
    AxrXrInteractionProfileEnum xrInteractionProfile
);
/// Get all the pose input actions for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The collection of pose input actions
[[nodiscard]] std::unordered_set<AxrPoseInputActionEnum> axrGetXrInteractionProfilePoseInputActionBindings(
    AxrXrInteractionProfileEnum xrInteractionProfile
);
/// Get all the haptic output actions for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The collection of haptic output actions
[[nodiscard]] std::unordered_set<AxrHapticOutputActionEnum> axrGetXrInteractionProfileHapticOutputActionBindings(
    AxrXrInteractionProfileEnum xrInteractionProfile
);

/// Get the names of all the action bindings for the given interaction profile
/// @param xrInteractionProfile Interaction profile
/// @returns The action xr path names
[[nodiscard]] std::unordered_set<const char*> axrGetXrInteractionProfileBindingNames(
    AxrXrInteractionProfileEnum xrInteractionProfile
);

/// Get the interaction profile name
/// @param interactionProfileEnum Interaction profile
/// @returns The interaction profile name
const char* axrGetXrInteractionProfileName(AxrXrInteractionProfileEnum interactionProfileEnum);
