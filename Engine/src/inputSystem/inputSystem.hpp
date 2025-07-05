#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/inputSystem.h"
#include "axr/common/enums.h"

/// Axr input system
class AxrInputSystem {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrInputSystem();
    /// Copy Constructor
    /// @param src Source AxrInputSystem to copy from
    AxrInputSystem(const AxrInputSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrInputSystem to move from
    AxrInputSystem(AxrInputSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrInputSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrInputSystem to copy from
    AxrInputSystem& operator=(const AxrInputSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrInputSystem to move from
    AxrInputSystem& operator=(AxrInputSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the input system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
};
