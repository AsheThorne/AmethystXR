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

    /// Set up the window system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Check if the window is open
    /// @returns True if the window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Open the window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult openWindow();
    /// Signal that we want to close the window
    void closeWindow();
    /// Process the window events
    void processEvents();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

#ifdef AXR_PLATFORM_WIN32
    // ---- Win32 Variables ----
    std::unique_ptr<AxrWin32WindowSystem> m_Win32WindowSystem;
#endif

    // ---- Config Variables ----
    AxrWindowPlatformEnum m_Platform;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Win32 Functions ----

    /// Set up the win32 window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupWin32Window();
    /// Check if the win32 window is open
    /// @returns True if the win32 window is open
    [[nodiscard]] bool isWin32WindowOpen() const;
    /// Open the win32 window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult openWin32Window();
    /// Signal that we want to close the win32 window
    void closeWin32Window();
    /// Process the win32 window events
    void processWin32Events();
};
