#pragma once
#ifdef AXR_USE_PLATFORM_WIN32

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "../../ioActionsSystem/ioActionsSystem.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <cstdint>

// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

/// Win32 Window System
class AxrWin32WindowSystem {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// On window resized callback function type
    /// @param 1: New window width 
    /// @param 2: New window height
    using OnWindowResizedCallback_T = AxrCallback<void(uint32_t, uint32_t)>;

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Win32 Window System Config
    struct Config {
        std::string ApplicationName;
        AxrIOActionsSystem_T IOActionsSystem;
        uint32_t Width;
        uint32_t Height;
        OnWindowResizedCallback_T OnWindowResizedCallback;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Win32 window config
    AxrWin32WindowSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrWin32WindowSystem to copy from
    AxrWin32WindowSystem(const AxrWin32WindowSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrWin32WindowSystem to move from
    AxrWin32WindowSystem(AxrWin32WindowSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrWin32WindowSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrWin32WindowSystem to copy from
    AxrWin32WindowSystem& operator=(const AxrWin32WindowSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrWin32WindowSystem to move from
    AxrWin32WindowSystem& operator=(AxrWin32WindowSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the win32 window system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Check if the window is open
    /// @returns True if the window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Open and show the window
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult openWindow();
    /// Signal that we want to close the window
    void closeWindow();
    /// Process the window message queue
    /// @returns False if the window was closed
    [[nodiscard]] bool processEvents();
    /// Get the window client size in pixels
    /// @param width Output window client width
    /// @param height Output window client height
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult getClientSize(uint32_t& width, uint32_t& height) const;

    /// Get the win32 instance
    /// @returns Win32 instance
    [[nodiscard]] HINSTANCE getInstance() const;
    /// Get the win32 window handle
    /// @returns Win32 window handle
    [[nodiscard]] HWND getWindowHandle() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_ApplicationName;
    AxrIOActionsSystem_T m_IOActionsSystem;
    uint32_t m_Width;
    uint32_t m_Height;
    OnWindowResizedCallback_T m_OnWindowResizedCallback;

    std::wstring m_WindowClassName;
    HINSTANCE m_Instance;
    HWND m_WindowHandle;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Destroy the window
    void destroyWin32Window();

    /// Process a win32 window message internally
    /// @param windowHandle Handle to the window
    /// @param uMsg Message
    /// @param wParam wParam
    /// @param lParam lParam
    /// @param wasHandled Output of whether the given message was handled
    /// @returns Message result (can represent something different depending on the message)
    LRESULT processWindowMessageInternal(
        HWND windowHandle,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam,
        bool& wasHandled
    );

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //

    /// Process a win32 window message
    /// @param windowHandle Handle to the window
    /// @param uMsg Message
    /// @param wParam wParam
    /// @param lParam lParam
    /// @returns Message result (can represent something different depending on the message)
    static LRESULT CALLBACK processWindowMessage(
        HWND windowHandle,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
    );
};

#endif
