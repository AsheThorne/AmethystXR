#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionSystem.h"
#include "boolInputAction.hpp"
#include "floatInputAction.hpp"
#include "vec2InputAction.hpp"
#include "../xrSystem/xrSystem.hpp"

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


    /// Set the priority over other action sets. Higher number = Higher priority.
    /// @param priority new priority
    void setPriority(uint32_t priority);
    /// Get the priority level
    /// @returns The priority level
    [[nodiscard]] uint32_t getPriority() const;

    /// Enable the input/output action set
    void enable();
    /// Disable the input/output action set
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

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the xr actions
    /// @param xrSystem Xr system to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrActions(AxrXrSystem_T xrSystem);
    /// Reset setupXrActions() function
    void resetSetupXrActions();

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

    /// Get the xr action set
    /// @returns The xr action set
    [[nodiscard]] XrActionSet getXrActionSet() const;

    /// Update the xr action values
    void updateXrActionValues();

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

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_LocalizedName;
    std::unordered_map<std::string, AxrBoolInputAction> m_BoolInputActions;
    std::unordered_map<std::string, AxrFloatInputAction> m_FloatInputActions;
    std::unordered_map<std::string, AxrVec2InputAction> m_Vec2InputActions;

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
