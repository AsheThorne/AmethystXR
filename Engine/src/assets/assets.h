#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

/// Axr Assets
class AxrAssets {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrAssets to copy from
    AxrAssets(const AxrAssets& src) = delete;
    /// Move Constructor
    /// @param src Source AxrAssets to move from
    AxrAssets(AxrAssets&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAssets to copy from
    AxrAssets& operator=(const AxrAssets& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAssets to move from
    AxrAssets& operator=(AxrAssets&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrAssets();

    // ---- Destructor ----

    /// Destructor
    ~AxrAssets();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrAssets Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrAssets singleton
    /// @return A reference to the AxrAssets singleton
    static AxrAssets& get();

    /// Set up the assets
    /// @param config Assets config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the assets
    void shutDown();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    bool m_IsSetup = false;
};
