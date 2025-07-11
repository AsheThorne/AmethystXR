#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionSystem.h"

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

    /// Check if the value was set this frame
    /// @returns True if the value was set this frame
    [[nodiscard]] bool wasValueSetThisFrame() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] float getValue() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Check if this input action contains the given binding
    /// @param biding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrFloatInputActionEnum biding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(float value);

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
    std::unordered_set<AxrFloatInputActionEnum> m_Bindings;

    // ---- Data ----
    float m_Value;
    bool m_WasTriggeredThisFrame;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
