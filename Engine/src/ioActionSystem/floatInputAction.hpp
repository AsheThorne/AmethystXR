#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionSystem.h"
#include "../xrSystem/xrSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_set>

/// Axr Float Input Action
class AxrFloatInputAction {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrFloatInputAction config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        AxrIOActionXrVisibilityEnum XrVisibility;
        uint32_t BindingCount;
        AxrFloatInputActionEnum* Bindings;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrFloatInputAction config
    explicit AxrFloatInputAction(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrFloatInputAction to copy from
    AxrFloatInputAction(const AxrFloatInputAction& src) = delete;
    /// Move Constructor
    /// @param src Source AxrFloatInputAction to move from
    AxrFloatInputAction(AxrFloatInputAction&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrFloatInputAction();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrFloatInputAction to copy from
    AxrFloatInputAction& operator=(const AxrFloatInputAction& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrFloatInputAction to move from
    AxrFloatInputAction& operator=(AxrFloatInputAction&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Enable the float action set
    void enable();
    /// Disable the float action set
    void disable();
    /// Check if the action is enabled
    /// @returns True if the action is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Check if the value has changed since the last frame
    /// @returns True if the value has changed since the last frame
    [[nodiscard]] bool valueChanged() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] float getValue() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the xr actions
    /// @param xrSystem Xr system to use
    /// @param actionSet Action set associated with this action
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrActions(AxrXrSystem_T xrSystem, XrActionSet actionSet);
    /// Reset setupXrActions() function
    void resetSetupXrActions();

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Get the XrAction
    /// @returns The XrAction
    [[nodiscard]] XrAction getXrAction() const;
    /// Get the bindings
    /// @returns The bindings
    [[nodiscard]] const std::unordered_set<AxrFloatInputActionEnum>& getBindings() const;
    /// Check if this input action contains the given binding
    /// @param biding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrFloatInputActionEnum biding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(float value);
    /// Reset the input action
    void reset();

    /// Check if this should be visible to the xr session
    /// @returns True if this should be visible to the xr session
    [[nodiscard]] bool isVisibleToXrSession() const;

    /// Update the xr action value
    void updateXrActionValue();

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given float input action config
    /// @param inputActionConfig Float input action config to clone
    /// @returns The cloned float input action
    [[nodiscard]] static AxrFloatInputActionConfig clone(const AxrFloatInputActionConfig& inputActionConfig);
    /// Destroy the given float input action config
    /// @param inputActionConfig Float input action config to destroy
    static void destroy(AxrFloatInputActionConfig& inputActionConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    AxrIOActionXrVisibilityEnum m_XrVisibility;
    std::unordered_set<AxrFloatInputActionEnum> m_Bindings;

    // ---- Data ----
    bool m_IsEnabled;
    float m_Value;
    float m_ValueLastFrame;
    AxrXrSystem_T m_XrSystem;
    XrAction m_XrAction;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
