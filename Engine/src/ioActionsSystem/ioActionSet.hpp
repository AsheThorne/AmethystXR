#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionsSystem.h"
#include "boolInputAction.hpp"
#include "floatInputAction.hpp"
#include "vec2InputAction.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_map>

/// Axr Input/Output Action Set
class AxrIOActionSet {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrIOActionSet Config
    struct Config {
        std::string Name;
        std::string LocalizedName;
        uint32_t BoolInputActionCount;
        AxrBoolInputActionConfig* BoolInputActions;
        uint32_t FloatInputActionCount;
        AxrFloatInputActionConfig* FloatInputActions;
        uint32_t Vec2InputActionCount;
        AxrVec2InputActionConfig* Vec2InputActions;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrIOActionSet config
    explicit AxrIOActionSet(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrIOActionSet to copy from
    AxrIOActionSet(const AxrIOActionSet& src) = delete;
    /// Move Constructor
    /// @param src Source AxrIOActionSet to move from
    AxrIOActionSet(AxrIOActionSet&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrIOActionSet();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrIOActionSet to copy from
    AxrIOActionSet& operator=(const AxrIOActionSet& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrIOActionSet to move from
    AxrIOActionSet& operator=(AxrIOActionSet&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Check if the action set is enabled
    /// @returns True if the action set is enabled
    [[nodiscard]] bool isEnabled() const;
    /// Get the set priority level
    /// @returns The set priority level
    [[nodiscard]] uint32_t getPriority() const;

    /// Get the bool input actions
    /// @returns The bool input actions
    [[nodiscard]] std::unordered_map<std::string, AxrBoolInputAction>& getBoolInputActions();
    /// Get the float input actions
    /// @returns The float input actions
    [[nodiscard]] std::unordered_map<std::string, AxrFloatInputAction>& getFloatInputActions();
    /// Get the vec2 input actions
    /// @returns The vec2 input actions
    [[nodiscard]] std::unordered_map<std::string, AxrVec2InputAction>& getVec2InputActions();

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given input/output action set config
    /// @param ioActionSetConfig Input/Output action set config to clone
    /// @returns The cloned input/output action set
    [[nodiscard]] static AxrIOActionSetConfig clone(const AxrIOActionSetConfig& ioActionSetConfig);
    /// Destroy the given input/output action set config
    /// @param ioActionSetConfig Input/Output action set config to destroy
    static void destroy(AxrIOActionSetConfig& ioActionSetConfig);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::string m_Name;
    std::string m_LocalizedName;
    bool m_IsEnabled;
    uint32_t m_Priority;
    std::unordered_map<std::string, AxrBoolInputAction> m_BoolInputActions;
    std::unordered_map<std::string, AxrFloatInputAction> m_FloatInputActions;
    std::unordered_map<std::string, AxrVec2InputAction> m_Vec2InputActions;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
