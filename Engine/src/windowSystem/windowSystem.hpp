#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/windowSystem.h"
#include "win32/win32WindowSystem.hpp"
#include "axr/actionSystem.h"

/// Axr Window System
class AxrWindowSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Window System Config
    struct Config {
        std::string ApplicationName;
        AxrActionSystem_T ActionSystem;
        uint32_t Width;
        uint32_t Height;
    };

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// On window open state changed callback function type
    /// @param 1: True if the window is open. False if it's not
    /// @returns AXR_SUCCESS if the function succeeded
    using OnWindowOpenStateChangedCallback_T = AxrCallback<AxrResult(bool)>;

    /// On window resized callback function type
    /// @param 1: New window width 
    /// @param 2: New window height
    using OnWindowResizedCallback_T = AxrCallback<void(uint32_t, uint32_t)>;

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //

    /// On window open state changed for the graphics system
    OnWindowOpenStateChangedCallback_T OnWindowOpenStateChangedCallbackGraphics;
    /// On window resized callback for the graphics system
    OnWindowResizedCallback_T OnWindowResizedCallbackGraphics;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Null config Constructor
    explicit AxrWindowSystem(std::nullptr_t);
    /// Constructor
    /// @param config Window config
    explicit AxrWindowSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrWindowSystem to copy from
    AxrWindowSystem(const AxrWindowSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrWindowSystem to move from
    AxrWindowSystem(AxrWindowSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrWindowSystem();

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

    /// Check if the window system is valid for use
    /// @returns True if the window system is valid for use
    [[nodiscard]] bool isValid() const;

    /// Check if the window is open
    /// @returns True if the window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Open the window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult openWindow();
    /// Signal that we want to close the window
    void closeWindow();
    /// Check if the cursor is hidden
    /// @returns True if the cursor is hidden
    [[nodiscard]] bool isCursorHidden() const;
    /// Unhide the cursor
    void showCursor();
    /// Hide the cursor
    void hideCursor();
    /// Check if the cursor's position is locked
    /// @returns True if the cursor's position is locked
    [[nodiscard]] bool isCursorLocked() const;
    /// Lock the cursor's position
    void lockCursor();
    /// Unlock the cursor's position
    void unlockCursor();

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the window system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

    /// Process the window events
    void processEvents();

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
    bool m_IsValid = false;

#ifdef AXR_USE_PLATFORM_WIN32
    // ---- Win32 Variables ----
    AxrWin32WindowSystem* m_Win32WindowSystem = nullptr;
#endif

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// 'On window resized' callback for the platform specific window system
    /// @param width New window width
    /// @param height New window height
    void onWindowResizedCallback(uint32_t width, uint32_t height) const;
};
