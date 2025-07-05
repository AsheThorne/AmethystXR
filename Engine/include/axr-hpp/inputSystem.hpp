#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/inputSystem.h"

namespace axr {
    // ----------------------------------------- //
    // Input System Definition
    // ----------------------------------------- //

    /// Input System
    class InputSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param inputSystem Input system handle
        InputSystem(const AxrInputSystem_T inputSystem):
            m_InputSystem(inputSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrInputSystem_T m_InputSystem;
    };
}
