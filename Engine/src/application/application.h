#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "axr/common/enums.h"

/// Axr Application singleton
class AxrApplication {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrApplication to copy from
    AxrApplication(const AxrApplication& src) = delete;
    /// Move Constructor
    /// @param src Source AxrApplication to move from
    AxrApplication(AxrApplication&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrApplication to copy from
    AxrApplication& operator=(const AxrApplication& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrApplication to move from
    AxrApplication& operator=(AxrApplication&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrApplication();

    // ---- Destructor ----

    /// Destructor
    ~AxrApplication();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrApplication Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrApplication singleton
    /// @return A reference to the AxrApplication singleton
    static AxrApplication& get();

    /// Set up the application
    /// @param config Application config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the application
    void shutDown();

    /// Check if the application is running
    /// @return True if the application is running
    [[nodiscard]] bool isRunning() const;
    /// Start a new frame.
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_APPLICATION_CLOSED if the main loop should exit and program should close.
    [[nodiscard]] AxrResult startNewFrame() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    bool m_IsSetup = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Process all application events
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_APPLICATION_CLOSED if the main loop should exit and program should close.
    [[nodiscard]] AxrResult processEvents() const;
};
