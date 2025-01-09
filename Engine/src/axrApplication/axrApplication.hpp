#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/axrApplication.h"
#include "../windowSystem/windowSystem.hpp"

/// AmethystXr Application
class AxrApplication {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Application config
    AxrApplication(const AxrApplicationConfig& config);
    /// Copy Constructor
    /// @param src Source AxrApplication to copy from
    AxrApplication(const AxrApplication& src) = delete;
    /// Move Constructor
    /// @param src Source AxrApplication to move from
    AxrApplication(AxrApplication&& src) = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrApplication() = default;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrApplication to copy from
    AxrApplication& operator=(const AxrApplication& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrApplication to move from
    AxrApplication& operator=(AxrApplication&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the AxrApplication class
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Check if the application is running
    /// @returns True if the application is running
    [[nodiscard]] bool isRunning() const;
    /// Process application events
    void processEvents();
    
    /// Get the window system
    /// @returns A handle to the window system
    [[nodiscard]] AxrWindowSystem_T getWindowSystem();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrApplicationConfig m_Config;
    AxrWindowSystem m_WindowSystem;
};
