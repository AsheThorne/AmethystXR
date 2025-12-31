#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

#include <SDL3/SDL.h>

/// AXR Platform singleton class
class AxrPlatform {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrPlatform to copy from
    AxrPlatform(const AxrPlatform& src) = delete;
    /// Move Constructor
    /// @param src Source AxrPlatform to move from
    AxrPlatform(AxrPlatform&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPlatform to copy from
    AxrPlatform& operator=(const AxrPlatform& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrPlatform to move from
    AxrPlatform& operator=(AxrPlatform&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrPlatform() = default;

    // ---- Destructor ----

    /// Destructor
    ~AxrPlatform() = default;

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrPlatform Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrPlatform singleton
    /// @return A reference to the AxrPlatform singleton
    static AxrPlatform& get();

    /// Set up the platform
    /// @param config Platform config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the platform
    void shutDown();

    /// Check if the platform is running
    /// @return True if the platform is running
    [[nodiscard]] bool isRunning() const;
    /// Process all platform events
    void processEvents();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    SDL_Window* m_SDLWindow = nullptr;
    bool m_IsSetup = false;
    bool m_IsRunning = false;

    /// Create the desktop window
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindow();
    /// Destroy the desktop window
    void destroyWindow();
};
