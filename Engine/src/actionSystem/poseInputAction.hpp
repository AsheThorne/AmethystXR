#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"
#include "../xrSystem/xrSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

/// Axr Pose Input Action
class AxrPoseInputAction {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrPoseInputAction config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        AxrActionXrVisibilityEnum XrVisibility;
        AxrPoseInputActionEnum Binding;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrPoseInputAction config
    explicit AxrPoseInputAction(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrPoseInputAction to copy from
    AxrPoseInputAction(const AxrPoseInputAction& src) = delete;
    /// Move Constructor
    /// @param src Source AxrPoseInputAction to move from
    AxrPoseInputAction(AxrPoseInputAction&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrPoseInputAction();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPoseInputAction to copy from
    AxrPoseInputAction& operator=(const AxrPoseInputAction& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrPoseInputAction to move from
    AxrPoseInputAction& operator=(AxrPoseInputAction&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Enable the pose action set
    void enable();
    /// Disable the pose action set
    void disable();
    /// Check if the action is enabled
    /// @returns True if the action is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] AxrPose getValue() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the xr action
    /// @param xrSystem Xr system to use
    /// @param actionSet Action set associated with this action
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrAction(AxrXrSystem_T xrSystem, XrActionSet actionSet);
    /// Reset setupXrAction() function
    void resetSetupXrAction();
    /// Create the xr space
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createXrSpace();
    /// Destroy the xr space
    void destroyXrSpace();

    /// Get the XrAction
    /// @returns The XrAction
    [[nodiscard]] XrAction getXrAction() const;
    /// Get the XrSpace
    /// @returns The XrSpace
    [[nodiscard]] XrSpace getXrSpace() const;
    /// Get the binding
    /// @returns The binding
    [[nodiscard]] AxrPoseInputActionEnum getBinding() const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(const AxrPose& value);
    /// Reset the input action
    void reset();

    /// Check if this should be visible to the xr session
    /// @returns True if this should be visible to the xr session
    [[nodiscard]] bool isVisibleToXrSession() const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given pose input action config
    /// @param inputActionConfig Pose input action config to clone
    /// @returns The cloned pose input action
    [[nodiscard]] static AxrPoseInputActionConfig clone(const AxrPoseInputActionConfig& inputActionConfig);
    /// Destroy the given pose input action config
    /// @param inputActionConfig Pose input action config to destroy
    static void destroy(AxrPoseInputActionConfig& inputActionConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    AxrActionXrVisibilityEnum m_XrVisibility;
    AxrPoseInputActionEnum m_Binding;

    // ---- Data ----
    bool m_IsEnabled;
    AxrPose m_Value;
    AxrXrSystem_T m_XrSystem;
    XrAction m_XrAction;
    XrSpace m_XrSpace;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
