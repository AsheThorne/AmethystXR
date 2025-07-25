﻿#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"
#include "../xrSystem/xrSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_set>

/// Axr Bool Input Action
class AxrBoolInputAction {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrBoolInputAction config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        AxrActionXrVisibilityEnum XrVisibility;
        uint32_t BindingCount;
        AxrBoolInputActionEnum* Bindings;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrBoolInputAction config
    explicit AxrBoolInputAction(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrBoolInputAction to copy from
    AxrBoolInputAction(const AxrBoolInputAction& src) = delete;
    /// Move Constructor
    /// @param src Source AxrBoolInputAction to move from
    AxrBoolInputAction(AxrBoolInputAction&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrBoolInputAction();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrBoolInputAction to copy from
    AxrBoolInputAction& operator=(const AxrBoolInputAction& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrBoolInputAction to move from
    AxrBoolInputAction& operator=(AxrBoolInputAction&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Enable the bool action set
    void enable();
    /// Disable the bool action set
    void disable();
    /// Check if the action is enabled
    /// @returns True if the action is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Check if the value has changed since the last frame
    /// @returns True if the value has changed since the last frame
    [[nodiscard]] bool valueChanged() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] bool getValue() const;

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

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Get the XrAction
    /// @returns The XrAction
    [[nodiscard]] XrAction getXrAction() const;
    /// Get the bindings
    /// @returns The bindings
    [[nodiscard]] const std::unordered_set<AxrBoolInputActionEnum>& getBindings() const;
    /// Check if this input action contains the given binding
    /// @param binding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrBoolInputActionEnum binding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(bool value);
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

    /// Clone the given bool input action config
    /// @param inputActionConfig Bool input action config to clone
    /// @returns The cloned bool input action
    [[nodiscard]] static AxrBoolInputActionConfig clone(const AxrBoolInputActionConfig& inputActionConfig);
    /// Destroy the given bool input action config
    /// @param inputActionConfig Bool input action config to destroy
    static void destroy(AxrBoolInputActionConfig& inputActionConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    AxrActionXrVisibilityEnum m_XrVisibility;
    std::unordered_set<AxrBoolInputActionEnum> m_Bindings;

    // ---- Data ----
    bool m_IsEnabled;
    bool m_Value;
    bool m_ValueLastFrame;
    AxrXrSystem_T m_XrSystem;
    XrAction m_XrAction;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
