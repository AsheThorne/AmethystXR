#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionsSystem.h"

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
