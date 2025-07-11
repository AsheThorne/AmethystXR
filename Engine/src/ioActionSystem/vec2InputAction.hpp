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

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>

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

    /// Check if the value was set this frame
    /// @returns True if the value was set this frame
    [[nodiscard]] bool wasValueSetThisFrame() const;

    /// Get the current value of this input action
    /// @returns The current value of this input action
    [[nodiscard]] glm::vec2 getValue() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Check if this input action contains the given binding
    /// @param biding Binding to check
    /// @returns True if this input action contains the given binding
    [[nodiscard]] bool containsBinding(AxrVec2InputActionEnum biding) const;

    /// Trigger the input action
    /// @param value Value to use
    void trigger(const glm::vec2& value);

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
    std::unordered_set<AxrVec2InputActionEnum> m_Bindings;

    // ---- Data ----
    glm::vec2 m_Value;
    bool m_WasTriggeredThisFrame;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
