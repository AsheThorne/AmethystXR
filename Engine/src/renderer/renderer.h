#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

/// Axr Renderer singleton
class AxrRenderer {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrRenderer to copy from
    AxrRenderer(const AxrRenderer& src) = delete;
    /// Move Constructor
    /// @param src Source AxrRenderer to move from
    AxrRenderer(AxrRenderer&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrRenderer to copy from
    AxrRenderer& operator=(const AxrRenderer& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrRenderer to move from
    AxrRenderer& operator=(AxrRenderer&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrRenderer();

    // ---- Destructor ----

    /// Destructor
    ~AxrRenderer();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrRenderer Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrRenderer singleton
    /// @return A reference to the AxrRenderer singleton
    static AxrRenderer& get();

    /// Set up the renderer
    /// @param config Renderer config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the renderer
    void shutDown();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    bool m_IsSetup = false;
};
