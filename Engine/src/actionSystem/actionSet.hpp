#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"
#include "boolInputAction.hpp"
#include "floatInputAction.hpp"
#include "vec2InputAction.hpp"
#include "poseInputAction.hpp"
#include "hapticOutputAction.hpp"
#include "../xrSystem/xrSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_map>

/// Axr Action Set
class AxrActionSet {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrActionSet Config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        uint32_t BoolInputActionCount;
        AxrBoolInputActionConfig* BoolInputActions;
        uint32_t FloatInputActionCount;
        AxrFloatInputActionConfig* FloatInputActions;
        uint32_t Vec2InputActionCount;
        AxrVec2InputActionConfig* Vec2InputActions;
        uint32_t PoseInputActionCount;
        AxrPoseInputActionConfig* PoseInputActions;
        uint32_t HapticOutputActionCount;
        AxrHapticOutputActionConfig* HapticOutputActions;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrActionSet config
    explicit AxrActionSet(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrActionSet to copy from
    AxrActionSet(const AxrActionSet& src) = delete;
    /// Move Constructor
    /// @param src Source AxrActionSet to move from
    AxrActionSet(AxrActionSet&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrActionSet();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrActionSet to copy from
    AxrActionSet& operator=(const AxrActionSet& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrActionSet to move from
    AxrActionSet& operator=(AxrActionSet&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //


    /// Set the priority over other action sets. Higher number = Higher priority.
    /// @param priority new priority
    void setPriority(uint32_t priority);
    /// Get the priority level
    /// @returns The priority level
    [[nodiscard]] uint32_t getPriority() const;

    /// Enable the action set
    void enable();
    /// Disable the action set
    void disable();
    /// Check if the action set is enabled
    /// @returns True if the action set is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Get the named bool input action
    /// @param name Bool input action name
    /// @returns The bool input action or nullptr if it wasn't found
    [[nodiscard]] AxrBoolInputAction_T getBoolInputAction(const std::string& name);
    /// Get the named float input action
    /// @param name Float input action name
    /// @returns The float input action or nullptr if it wasn't found
    [[nodiscard]] AxrFloatInputAction_T getFloatInputAction(const std::string& name);
    /// Get the named vec2 input action
    /// @param name Vec2 input action name
    /// @returns The vec2 input action or nullptr if it wasn't found
    [[nodiscard]] AxrVec2InputAction_T getVec2InputAction(const std::string& name);
    /// Get the named pose input action
    /// @param name Pose input action name
    /// @returns The pose input action or nullptr if it wasn't found
    [[nodiscard]] AxrPoseInputAction_T getPoseInputAction(const std::string& name);
    /// Get the named haptic output action
    /// @param name Haptic output action name
    /// @returns The haptic output action or nullptr if it wasn't found
    [[nodiscard]] AxrHapticOutputAction_T getHapticOutputAction(const std::string& name);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the xr actions
    /// @param xrSystem Xr system to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrActions(AxrXrSystem_T xrSystem);
    /// Reset setupXrActions() function
    void resetSetupXrActions();
    /// Create the spaces
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createXrSpaces();
    /// Destroy the xr spaces
    void destroyXrSpaces();

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Get the bool input actions
    /// @returns The bool input actions
    [[nodiscard]] std::unordered_map<std::string, AxrBoolInputAction>& getBoolInputActions();
    /// Get the float input actions
    /// @returns The float input actions
    [[nodiscard]] std::unordered_map<std::string, AxrFloatInputAction>& getFloatInputActions();
    /// Get the vec2 input actions
    /// @returns The vec2 input actions
    [[nodiscard]] std::unordered_map<std::string, AxrVec2InputAction>& getVec2InputActions();
    /// Get the pose input actions
    /// @returns The pose input actions
    [[nodiscard]] std::unordered_map<std::string, AxrPoseInputAction>& getPoseInputActions();
    /// Get the haptic output actions
    /// @returns The haptic output actions
    [[nodiscard]] std::unordered_map<std::string, AxrHapticOutputAction>& getHapticOutputActions();

    /// Check if this action set contains the given bool binding
    /// @param binding Binding to check
    /// @returns True if this action set contains the given bool binding
    [[nodiscard]] bool containsBinding(AxrBoolInputActionEnum binding) const;
    /// Check if this action set contains the given float binding
    /// @param binding Binding to check
    /// @returns True if this action set contains the given float binding
    [[nodiscard]] bool containsBinding(AxrFloatInputActionEnum binding) const;
    /// Check if this action set contains the given vec2 binding
    /// @param binding Binding to check
    /// @returns True if this action set contains the given vec2 binding
    [[nodiscard]] bool containsBinding(AxrVec2InputActionEnum binding) const;
    /// Check if this action set contains the given pose binding
    /// @param binding Binding to check
    /// @returns True if this action set contains the given pose binding
    [[nodiscard]] bool containsBinding(AxrPoseInputActionEnum binding) const;
    /// Check if this action set contains the given haptic binding
    /// @param binding Binding to check
    /// @returns True if this action set contains the given haptic binding
    [[nodiscard]] bool containsBinding(AxrHapticOutputActionEnum binding) const;

    /// Trigger the given bool input action
    /// @param inputActionEnum Bool input action
    /// @param value Bool value
    void triggerBoolInputAction(AxrBoolInputActionEnum inputActionEnum, bool value);
    /// Trigger the given float input action
    /// @param inputActionEnum Float input action
    /// @param value Float value
    void triggerFloatInputAction(AxrFloatInputActionEnum inputActionEnum, float value);
    /// Trigger the given vec2 input action
    /// @param inputActionEnum Vec2 input action
    /// @param value Vec2 value
    void triggerVec2InputAction(AxrVec2InputActionEnum inputActionEnum, const AxrVec2& value);
    /// Trigger the given pose input action
    /// @param inputActionEnum Pose input action
    /// @param value Pose value
    void triggerPoseInputAction(AxrPoseInputActionEnum inputActionEnum, const AxrPose& value);

    /// Reset the given bool input action
    /// @param inputActionEnum Bool input action
    void resetBoolInputAction(AxrBoolInputActionEnum inputActionEnum);
    /// Reset the given float input action
    /// @param inputActionEnum Float input action
    void resetFloatInputAction(AxrFloatInputActionEnum inputActionEnum);
    /// Reset the given vec2 input action
    /// @param inputActionEnum Vec2 input action
    void resetVec2InputAction(AxrVec2InputActionEnum inputActionEnum);
    /// Reset the given pose input action
    /// @param inputActionEnum Pose input action
    void resetPoseInputAction(AxrPoseInputActionEnum inputActionEnum);

    /// Get the xr action set
    /// @returns The xr action set
    [[nodiscard]] XrActionSet getXrActionSet() const;

    /// Update the xr action values
    void updateXrActionValues();

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given action set config
    /// @param actionSetConfig Action set config to clone
    /// @returns The cloned action set
    [[nodiscard]] static AxrActionSetConfig clone(const AxrActionSetConfig& actionSetConfig);
    /// Destroy the given action set config
    /// @param actionSetConfig Action set config to destroy
    static void destroy(AxrActionSetConfig& actionSetConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    std::unordered_map<std::string, AxrBoolInputAction> m_BoolInputActions;
    std::unordered_map<std::string, AxrFloatInputAction> m_FloatInputActions;
    std::unordered_map<std::string, AxrVec2InputAction> m_Vec2InputActions;
    std::unordered_map<std::string, AxrPoseInputAction> m_PoseInputActions;
    std::unordered_map<std::string, AxrHapticOutputAction> m_HapticOutputActions;

    // ---- Data ----
    bool m_IsEnabled;
    uint32_t m_Priority;
    AxrXrSystem_T m_XrSystem;
    XrActionSet m_XrActionSet;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Check if this should be visible to the xr session
    /// @returns True if this should be visible to the xr session
    [[nodiscard]] bool isVisibleToXrSession() const;
};
