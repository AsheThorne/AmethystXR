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
#include <unordered_set>

/// Axr Vec2 Input Action
class AxrVec2InputAction {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrVec2InputAction config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        AxrActionXrVisibilityEnum XrVisibility;
        uint32_t BindingCount;
        AxrVec2InputActionEnum* Bindings;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrVec2InputAction config
    explicit AxrVec2InputAction(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVec2InputAction to copy from
    AxrVec2InputAction(const AxrVec2InputAction& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVec2InputAction to move from
    AxrVec2InputAction(AxrVec2InputAction&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVec2InputAction();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVec2InputAction to copy from
    AxrVec2InputAction& operator=(const AxrVec2InputAction& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVec2InputAction to move from
    AxrVec2InputAction& operator=(AxrVec2InputAction&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Enable the vec2 action set
    void enable();
    /// Disable the vec2 action set
    void disable();
    /// Check if the action is enabled
    /// @returns True if the action is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Check if the value has changed since the last frame
    /// @returns True if the value has changed since the last frame
    [[nodiscard]] bool valueChanged() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] AxrVec2 getValue() const;

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
    [[nodiscard]] const std::unordered_set<AxrVec2InputActionEnum>& getBindings() const;
    /// Check if this input action contains the given binding
    /// @param biding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrVec2InputActionEnum biding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(const AxrVec2& value);
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

    /// Clone the given vec2 input action config
    /// @param inputActionConfig Vec2 input action config to clone
    /// @returns The cloned vec2 input action
    [[nodiscard]] static AxrVec2InputActionConfig clone(const AxrVec2InputActionConfig& inputActionConfig);
    /// Destroy the given vec2 input action config
    /// @param inputActionConfig Vec2 input action config to destroy
    static void destroy(AxrVec2InputActionConfig& inputActionConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    AxrActionXrVisibilityEnum m_XrVisibility;
    std::unordered_set<AxrVec2InputActionEnum> m_Bindings;

    // ---- Data ----
    bool m_IsEnabled;
    AxrVec2 m_Value;
    AxrVec2 m_ValueLastFrame;
    AxrXrSystem_T m_XrSystem;
    XrAction m_XrAction;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
