#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

/// Axr Server singleton
class AxrServer {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrServer to copy from
    AxrServer(const AxrServer& src) = delete;
    /// Move Constructor
    /// @param src Source AxrServer to move from
    AxrServer(AxrServer&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrServer to copy from
    AxrServer& operator=(const AxrServer& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrServer to move from
    AxrServer& operator=(AxrServer&& src) noexcept = delete;
    
private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrServer();

    // ---- Destructor ----

    /// Destructor
    ~AxrServer();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrServer Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrServer singleton
    /// @return A reference to the AxrServer singleton
    static AxrServer& get();

    /// Set up the server
    /// @param config Server config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the server
    void shutDown();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    bool m_IsSetup = false;
};
