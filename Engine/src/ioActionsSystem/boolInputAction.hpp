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

    /// Check if this input action contains the given binding
    /// @param biding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrBoolInputActionEnum biding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(bool value);

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
    std::unordered_set<AxrBoolInputActionEnum> m_Bindings;

    // ---- Data ----
    bool m_Value;
    bool m_WasTriggeredThisFrame;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
