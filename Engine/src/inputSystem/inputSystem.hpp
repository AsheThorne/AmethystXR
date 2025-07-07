#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/inputSystem.h"
#include "axr/common/enums.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>
#include <chrono>
#include "unordered_set"

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif

/// Axr input system
class AxrInputSystem {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrInputSystem();
    /// Copy Constructor
    /// @param src Source AxrInputSystem to copy from
    AxrInputSystem(const AxrInputSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrInputSystem to move from
    AxrInputSystem(AxrInputSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrInputSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrInputSystem to copy from
    AxrInputSystem& operator=(const AxrInputSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrInputSystem to move from
    AxrInputSystem& operator=(AxrInputSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the input system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

#ifdef AXR_USE_PLATFORM_WIN32
    // ---- Win32 Functions ----

    /// Process a win32 window message
    /// @param windowHandle Handle to the window
    /// @param uMsg Message
    /// @param wParam wParam
    /// @param lParam lParam
    /// @param wasHandled Output of whether the given message was handled
    /// @returns Message result (can represent something different depending on the message)
    [[nodiscard]] LRESULT processWin32Message(
        HWND windowHandle,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam,
        bool& wasHandled
    );
#endif

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    uint32_t m_DoubleClickTime;
    std::unordered_set<AxrBoolInputActionEnum> m_ActiveBoolInputActions;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickLStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickRStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickMStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickX1StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickX2StartTime;
    glm::vec2 m_LastAbsoluteCursorPosition;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Trigger a bool input action
    /// @param inputActionEnum Bool input action
    /// @param value Bool value
    void triggerBoolInputAction(AxrBoolInputActionEnum inputActionEnum, bool value);
    /// Trigger a float input action
    /// @param inputActionEnum Float input action
    /// @param value Float value
    void triggerFloatInputAction(AxrFloatInputActionEnum inputActionEnum, float value);
    /// Trigger a vec2 input action
    /// @param inputActionEnum Vec2 input action
    /// @param value Vec2 value
    void triggerVec2InputAction(AxrVec2InputActionEnum inputActionEnum, glm::vec2 value);

    /// Clear all input action data
    void clearInputActions();

#ifdef AXR_USE_PLATFORM_WIN32
    // ---- Win32 Functions ----

    /// Set up win32 inputs
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupWin32Inputs();
    /// Reset setupWin32Inputs()
    void resetSetupWin32Inputs();

    /// Register win32 raw inputs
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult registerWin32RawInputs() const;

    /// Process win32 raw mouse input
    /// @param windowHandle Window handle
    /// @param rawInput Raw input data
    /// @param wasHandled Output of whether the given message was handled
    /// @returns Message result (can represent something different depending on the message)
    [[nodiscard]] LRESULT processWin32MouseInput(HWND windowHandle, RAWINPUT* rawInput, bool& wasHandled);
    /// Process win32 mouse moved input
    /// @param windowHandle Window handle
    /// @param rawInput Raw input data
    void processWin32MouseMovedInput(HWND windowHandle, const RAWINPUT* rawInput);
    /// Process win32 mouse down input
    /// @param rawInput Raw input data
    void processWin32MouseDownInput(const RAWINPUT* rawInput);
    /// Process win32 mouse up input
    /// @param rawInput Raw input data
    void processWin32MouseUpInput(const RAWINPUT* rawInput);
    /// Process win32 mouse scroll input
    /// @param rawInput Raw input data
    void processWin32MouseScrollInput(const RAWINPUT* rawInput);
#endif
};
