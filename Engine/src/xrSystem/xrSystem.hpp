#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/xrSystem.h"

/// Axr Xr System
class AxrXrSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Xr System Config
    struct Config {
        const char* ApplicationName;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Xr system config
    AxrXrSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrXrSystem to copy from
    AxrXrSystem(const AxrXrSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrXrSystem to move from
    AxrXrSystem(AxrXrSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrXrSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrXrSystem to copy from
    AxrXrSystem& operator=(const AxrXrSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrXrSystem to move from
    AxrXrSystem& operator=(AxrXrSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the xr session is running
    /// @returns True if the xr session is running
    [[nodiscard]] bool isXrSessionRunning() const;

    /// Start the xr session
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult startXrSession();

    /// Signal that we want to stop the xr session
    void stopXrSession();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_ApplicationName;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
};
