#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
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
    AxrPlatform();

    // ---- Destructor ----

    /// Destructor
    ~AxrPlatform();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrPlatform Config
    struct Config {
        uint32_t WindowWidth;
        uint32_t WindowHeight;
        char WindowTitle[AXR_MAX_WINDOW_TITLE_SIZE];
        bool WindowEnabled;
    };

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

    /// Check if the platform window is open
    /// @return True if the platform window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Process all platform events
    /// @return False if the window closed
    [[nodiscard]] bool processEvents();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    SDL_Window* m_SDLWindow = nullptr;
    bool m_IsSetup = false;
    bool m_IsWindowOpen = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Create the desktop window
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindow(const char (&title)[AXR_MAX_WINDOW_TITLE_SIZE],
                                         uint32_t width,
                                         uint32_t height);
    /// Destroy the desktop window
    void destroyWindow();

    /// Handle the given window event
    /// @param event Window event to handle
    void handleWindowEvent(const SDL_WindowEvent& event);
};
