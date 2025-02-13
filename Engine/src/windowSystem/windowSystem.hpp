#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/windowSystem.h"
#include "win32/win32WindowSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <memory>

/// Axr Window System
class AxrWindowSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Window System Config
    struct Config {
        const char* ApplicationName;
        AxrWindowSystemConfig WindowConfig;
    };

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// ConfigureWindowGraphics callback function type
    /// @param userData User data
    /// @param isWindowOpen If true,the graphics should be set up. If false, the graphics should be cleaned up.
    /// @returns AXR_SUCCESS if the function succeeded
    using ConfigureWindowGraphicsCallback_T = AxrResult(*)(void* userData, bool isWindowOpen);

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Window config
    AxrWindowSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrWindowSystem to copy from
    AxrWindowSystem(const AxrWindowSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrWindowSystem to move from
    AxrWindowSystem(AxrWindowSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrWindowSystem() = default;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrWindowSystem to copy from
    AxrWindowSystem& operator=(const AxrWindowSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrWindowSystem to move from
    AxrWindowSystem& operator=(AxrWindowSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the window is open
    /// @returns True if the window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Open the window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult openWindow();
    /// Signal that we want to close the window
    void closeWindow();

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They should not be given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the window system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Process the window events
    void processEvents();

    /// Set the 'ConfigureWindowGraphics' callback function
    /// @param userData User data
    /// @param function Callback function
    void setConfigureWindowGraphicsCallback(void* userData, ConfigureWindowGraphicsCallback_T function);
    /// Reset the 'ConfigureWindowGraphics' callback function
    void resetConfigureWindowGraphicsCallback();

#ifdef AXR_USE_PLATFORM_WIN32
    [[nodiscard]] AxrWin32WindowSystem* getWin32WindowSystem() const;
#endif

    /// Get the window client size in pixels
    /// @param width Output window client width
    /// @param height Output window client height
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult getClientSize(uint32_t& width, uint32_t& height) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

#ifdef AXR_USE_PLATFORM_WIN32
    // ---- Win32 Variables ----
    std::unique_ptr<AxrWin32WindowSystem> m_Win32WindowSystem;
#endif

    void* m_ConfigureWindowGraphicsCallbackUserData;
    ConfigureWindowGraphicsCallback_T m_ConfigureWindowGraphicsCallback;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Invoke 'Configure window graphics' callback
    /// @param isWindowOpen Window open state
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult invokeConfigureWindowGraphicsCallback(bool isWindowOpen) const;
};
