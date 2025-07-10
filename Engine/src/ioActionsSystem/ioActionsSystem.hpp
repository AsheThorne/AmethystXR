#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionsSystem.h"
#include "axr/common/enums.h"
#include "ioActionSet.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>
#include <chrono>
#include <unordered_set>
#include <unordered_map>

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

/// Axr Input/Output Actions System
class AxrIOActionsSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrIOActionsSystem config
    struct Config {
        uint32_t ActionSetCount;
        AxrIOActionSetConfig* ActionSets;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrIOActionsSystem config
    AxrIOActionsSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrIOActionsSystem to copy from
    AxrIOActionsSystem(const AxrIOActionsSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrIOActionsSystem to move from
    AxrIOActionsSystem(AxrIOActionsSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrIOActionsSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrIOActionsSystem to copy from
    AxrIOActionsSystem& operator=(const AxrIOActionsSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrIOActionsSystem to move from
    AxrIOActionsSystem& operator=(AxrIOActionsSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the input/output actions system
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
    std::unordered_map<std::string, AxrIOActionSet> m_ActionSets;
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
    [[nodiscard]] LRESULT processWin32MouseInput(HWND windowHandle, const RAWINPUT* rawInput, bool& wasHandled);
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
