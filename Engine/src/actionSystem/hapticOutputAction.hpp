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

/// Axr Haptic Output Action
class AxrHapticOutputAction {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrHapticOutputAction config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        AxrActionXrVisibilityEnum XrVisibility;
        uint32_t BindingCount;
        AxrHapticOutputActionEnum* Bindings;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrHapticOutputAction config
    explicit AxrHapticOutputAction(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrHapticOutputAction to copy from
    AxrHapticOutputAction(const AxrHapticOutputAction& src) = delete;
    /// Move Constructor
    /// @param src Source AxrHapticOutputAction to move from
    AxrHapticOutputAction(AxrHapticOutputAction&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrHapticOutputAction();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrHapticOutputAction to copy from
    AxrHapticOutputAction& operator=(const AxrHapticOutputAction& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrHapticOutputAction to move from
    AxrHapticOutputAction& operator=(AxrHapticOutputAction&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Enable the haptic action set
    void enable();
    /// Disable the haptic action set
    void disable();
    /// Check if the action is enabled
    /// @returns True if the action is enabled
    [[nodiscard]] bool isEnabled() const;

    /// Activate the action haptics
    /// @param duration Haptic duration in nanoseconds
    /// @param frequency Haptic frequency in Hz
    /// @param amplitude Haptic amplitude from 0.0f to 1.0f
    void activate(int64_t duration, float frequency, float amplitude);
    /// Deactivate the action haptics
    void deactivate();

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

    /// Get the XrAction
    /// @returns The XrAction
    [[nodiscard]] XrAction getXrAction() const;
    /// Get the bindings
    /// @returns The bindings
    [[nodiscard]] const std::unordered_set<AxrHapticOutputActionEnum>& getBindings() const;
    /// Check if this output action contains the given binding
    /// @param binding Binding to check
    /// @returns True if this output action contains the given binding
    [[nodiscard]] bool containsBinding(AxrHapticOutputActionEnum binding) const;

    /// Check if this should be visible to the xr session
    /// @returns True if this should be visible to the xr session
    [[nodiscard]] bool isVisibleToXrSession() const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given haptic output action config
    /// @param outputActionConfig Haptic output action config to clone
    /// @returns The cloned haptic output action
    [[nodiscard]] static AxrHapticOutputActionConfig clone(const AxrHapticOutputActionConfig& outputActionConfig);
    /// Destroy the given haptic output action config
    /// @param outputActionConfig Haptic output action config to destroy
    static void destroy(AxrHapticOutputActionConfig& outputActionConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    AxrActionXrVisibilityEnum m_XrVisibility;
    std::unordered_set<AxrHapticOutputActionEnum> m_Bindings;

    // ---- Data ----
    bool m_IsEnabled;
    AxrXrSystem_T m_XrSystem;
    XrAction m_XrAction;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
