#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/actionSystem.h"
#include "axr/common/enums.h"
#include "actionSet.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include "../xrSystem/xrSystem.hpp"

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif

/// Axr Action System
class AxrActionSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrActionSystem config
    struct Config {
        AxrXrSystem_T XrSystem;
        uint32_t ActionSetCount;
        AxrActionSetConfig* ActionSets;
        uint32_t XrInteractionProfileCount;
        AxrXrInteractionProfileEnum* XrInteractionProfiles;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config AxrActionSystem config
    AxrActionSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrActionSystem to copy from
    AxrActionSystem(const AxrActionSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrActionSystem to move from
    AxrActionSystem(AxrActionSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrActionSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrActionSystem to copy from
    AxrActionSystem& operator=(const AxrActionSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrActionSystem to move from
    AxrActionSystem& operator=(AxrActionSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the named action set
    /// @param name Action set name
    /// @returns The action set or nullptr if it wasn't found
    AxrActionSet_T getActionSet(const std::string& name);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the action system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

    /// Signal that a new frame has started
    void newFrameStarted();

    /// Process the action events
    void processEvents();

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

    // ---- Config Variables ----
    AxrXrSystem_T m_XrSystem;
    std::unordered_map<std::string, AxrActionSet> m_ActionSets;
    std::unordered_set<AxrXrInteractionProfileEnum> m_XrInteractionProfiles;

    // ---- Data ----
    uint32_t m_DoubleClickTime;
    std::unordered_set<AxrBoolInputActionEnum> m_ActiveBoolInputActions;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickLStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickRStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickMStartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickX1StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_MouseClickX2StartTime;
    AxrVec2 m_LastAbsoluteCursorPosition;

    // ---- Relative action data ----
    // Relative actions are actions that are relative to the last frame
    float m_ScrollDelta;
    float m_HorizontalScrollDelta;
    AxrVec2 m_MouseMovedDelta;

    std::vector<XrActionSet> m_XrActionSets;
    bool m_AreXrActionsAttached;

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
    void triggerVec2InputAction(AxrVec2InputActionEnum inputActionEnum, const AxrVec2& value);

    /// Reset a bool input action
    /// @param inputActionEnum Bool input action
    void resetBoolInputAction(AxrBoolInputActionEnum inputActionEnum);
    /// Reset a float input action
    /// @param inputActionEnum Float input action
    void resetFloatInputAction(AxrFloatInputActionEnum inputActionEnum);
    /// Reset a vec2 input action
    /// @param inputActionEnum Vec2 input action
    void resetVec2InputAction(AxrVec2InputActionEnum inputActionEnum);

    /// Trigger all relative actions
    void triggerRelativeActions();
    /// Reset all relative actions
    void resetRelativeActions();

    /// Clear all input action data
    void clearInputActions();

    // ---- XR Functions ----

    /// Set up xr inputs
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrInputs();
    /// Reset setupXrInputs()
    void resetSetupXrInputs();
    /// Find all the xr action sets
    /// @returns The collection xr action sets
    [[nodiscard]] std::vector<XrActionSet> findXrActionSets() const;

    /// Suggest bindings to the xr session
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult suggestXrBindings();

    /// 'On xr session state changed' callback function
    /// @param isSessionRunning If true, the xr session is running. If false, the xr session is not running.
    /// @returns AXR_SUCCESS if the function succeeded
    AxrResult onXrSessionStateChangedCallback(bool isSessionRunning);

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

    /// Process win32 key down input
    /// @param wParam wParam
    /// @param wasHandled Output of whether the given message was handled
    /// @returns Message result (can represent something different depending on the message)
    [[nodiscard]] LRESULT processWin32KeyDown(WPARAM wParam, bool& wasHandled);
    /// Process win32 key down input
    /// @param wParam wParam
    /// @param wasHandled Output of whether the given message was handled
    /// @returns Message result (can represent something different depending on the message)
    [[nodiscard]] LRESULT processWin32KeyUp(WPARAM wParam, bool& wasHandled);
#endif
};
