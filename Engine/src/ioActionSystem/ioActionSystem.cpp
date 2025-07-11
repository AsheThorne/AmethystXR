// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "ioActionSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <ranges>
#include <vector>

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <hidusage.h>
#endif

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrIOActionSet_T axrIOActionSystemGetIOActionSet(const AxrIOActionSystem_T ioActionSystem, const char* name) {
    if (ioActionSystem == nullptr) {
        axrLogErrorLocation("`ioActionSystem` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return ioActionSystem->getIOActionSet(name);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrIOActionSystem::AxrIOActionSystem(const Config& config):
    m_DoubleClickTime(0),
    m_LastAbsoluteCursorPosition(0.0f) {
    if (config.ActionSets != nullptr) {
        for (uint32_t i = 0; i < config.ActionSetCount; ++i) {
            m_ActionSets.insert(
                std::pair(
                    config.ActionSets[i].Name,
                    AxrIOActionSet(
                        AxrIOActionSet::Config{
                            .Name = config.ActionSets[i].Name,
                            .LocalizedName = config.ActionSets[i].LocalizedName,
                            .BoolInputActionCount = config.ActionSets[i].BoolInputActionCount,
                            .BoolInputActions = config.ActionSets[i].BoolInputActions,
                            .FloatInputActionCount = config.ActionSets[i].FloatInputActionCount,
                            .FloatInputActions = config.ActionSets[i].FloatInputActions,
                            .Vec2InputActionCount = config.ActionSets[i].Vec2InputActionCount,
                            .Vec2InputActions = config.ActionSets[i].Vec2InputActions,
                        }
                    )
                )
            );
        }
    }
}

AxrIOActionSystem::~AxrIOActionSystem() {
    resetSetup();
}

// ---- Public Functions ----

AxrIOActionSet_T AxrIOActionSystem::getIOActionSet(const std::string& name) {
    const auto foundIOActionSet = m_ActionSets.find(name);
    if (foundIOActionSet == m_ActionSets.end()) {
        return nullptr;
    }

    return &foundIOActionSet->second;
}

AxrResult AxrIOActionSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

#ifdef AXR_USE_PLATFORM_WIN32
    axrResult = setupWin32Inputs();
    if (AXR_FAILED(axrResult)) return axrResult;
#endif

    return AXR_SUCCESS;
}

void AxrIOActionSystem::resetSetup() {
    clearInputActions();

#ifdef AXR_USE_PLATFORM_WIN32
    resetSetupWin32Inputs();
#endif
}

void AxrIOActionSystem::newFrameStarted() {
    for (AxrIOActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        actionSet.newFrameStarted();
    }
}

#ifdef AXR_USE_PLATFORM_WIN32
// ReSharper disable once CppDFAConstantFunctionResult
LRESULT AxrIOActionSystem::processWin32Message(
    const HWND windowHandle,
    const UINT uMsg,
    const WPARAM wParam,
    const LPARAM lParam,
    bool& wasHandled
) {
    if (wasHandled) return 0;

    switch (uMsg) {
        case WM_INPUT: {
            uint32_t rawInputDataSize = sizeof(RAWINPUT);
            BYTE rawInputDataPointer[sizeof(RAWINPUT)];

            GetRawInputData(
                // NOLINTNEXTLINE(performance-no-int-to-ptr)
                reinterpret_cast<HRAWINPUT>(lParam),
                RID_INPUT,
                rawInputDataPointer,
                &rawInputDataSize,
                sizeof(RAWINPUTHEADER)
            );

            const auto rawInput = reinterpret_cast<RAWINPUT*>(rawInputDataPointer);

            if (rawInput->header.dwType == RIM_TYPEMOUSE) {
                const LRESULT result = processWin32MouseInput(windowHandle, rawInput, wasHandled);
                if (wasHandled) {
                    return result;
                }
            }
            break;
        }
        default: {
            return 0;
        }
    }

    return 0;
}
#endif

// ---- Private Functions ----

void AxrIOActionSystem::triggerBoolInputAction(const AxrBoolInputActionEnum inputActionEnum, const bool value) {
    if (value) {
        m_ActiveBoolInputActions.insert(inputActionEnum);
    } else {
        m_ActiveBoolInputActions.erase(inputActionEnum);
    }

    uint32_t highestPriority = 0;
    std::vector<AxrBoolInputAction*> inputActionsToTrigger;

    for (AxrIOActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (actionSet.isEnabled()) {
            // Only trigger actions on sets of the highest priority
            const uint32_t priority = actionSet.getPriority();
            if (priority > highestPriority) {
                inputActionsToTrigger.clear();
                highestPriority = priority;
            }

            if (priority != highestPriority) continue;

            for (AxrBoolInputAction& inputAction : actionSet.getBoolInputActions() | std::ranges::views::values) {
                if (inputAction.containsBinding(inputActionEnum)) {
                    inputActionsToTrigger.push_back(&inputAction);
                }
            }
        }
    }

    for (AxrBoolInputAction* inputAction : inputActionsToTrigger) {
        inputAction->trigger(value);
    }
}

void AxrIOActionSystem::triggerFloatInputAction(const AxrFloatInputActionEnum inputActionEnum, const float value) {
    uint32_t highestPriority = 0;
    std::vector<AxrFloatInputAction*> inputActionsToTrigger;

    for (AxrIOActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (actionSet.isEnabled()) {
            // Only trigger actions on sets of the highest priority
            const uint32_t priority = actionSet.getPriority();
            if (priority > highestPriority) {
                inputActionsToTrigger.clear();
                highestPriority = priority;
            }

            if (priority != highestPriority) continue;

            for (AxrFloatInputAction& inputAction : actionSet.getFloatInputActions() | std::ranges::views::values) {
                if (inputAction.containsBinding(inputActionEnum)) {
                    inputActionsToTrigger.push_back(&inputAction);
                }
            }
        }
    }

    for (AxrFloatInputAction* inputAction : inputActionsToTrigger) {
        inputAction->trigger(value);
    }
}

void AxrIOActionSystem::triggerVec2InputAction(const AxrVec2InputActionEnum inputActionEnum, const AxrVec2& value) {
    uint32_t highestPriority = 0;
    std::vector<AxrVec2InputAction*> inputActionsToTrigger;

    for (AxrIOActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (actionSet.isEnabled()) {
            // Only trigger actions on sets of the highest priority
            const uint32_t priority = actionSet.getPriority();
            if (priority > highestPriority) {
                inputActionsToTrigger.clear();
                highestPriority = priority;
            }

            if (priority != highestPriority) continue;

            for (AxrVec2InputAction& inputAction : actionSet.getVec2InputActions() | std::ranges::views::values) {
                if (inputAction.containsBinding(inputActionEnum)) {
                    inputActionsToTrigger.push_back(&inputAction);
                }
            }
        }
    }

    for (AxrVec2InputAction* inputAction : inputActionsToTrigger) {
        inputAction->trigger(value);
    }
}

void AxrIOActionSystem::clearInputActions() {
    m_ActiveBoolInputActions.clear();
    m_MouseClickLStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickRStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickMStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickX1StartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickX2StartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_LastAbsoluteCursorPosition = AxrVec2(0.0f, 0.0f);
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrResult AxrIOActionSystem::setupWin32Inputs() {
    const AxrResult axrResult = registerWin32RawInputs();
    if (AXR_FAILED(axrResult)) return axrResult;

    m_DoubleClickTime = GetDoubleClickTime();

    return AXR_SUCCESS;
}

void AxrIOActionSystem::resetSetupWin32Inputs() {
    m_DoubleClickTime = 0;
}

AxrResult AxrIOActionSystem::registerWin32RawInputs() const {
    const std::vector rawInputDevices{
        // Mouse 
        RAWINPUTDEVICE{
            .usUsagePage = HID_USAGE_PAGE_GENERIC,
            .usUsage = HID_USAGE_GENERIC_MOUSE,
            .dwFlags = 0,
            .hwndTarget = nullptr,
        }
    };

    // Register devices
    const BOOL result = RegisterRawInputDevices(
        rawInputDevices.data(),
        static_cast<uint32_t>(rawInputDevices.size()),
        sizeof(rawInputDevices[0])
    );

    // Check if the registration failed
    if (result == FALSE) {
        const DWORD error = GetLastError();
        axrLogErrorLocation("Failed to register win32 raw inputs. Error code: {0}.", error);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

// ReSharper disable once CppDFAConstantFunctionResult
LRESULT AxrIOActionSystem::processWin32MouseInput(const HWND windowHandle, const RAWINPUT* rawInput, bool& wasHandled) {
    // Only process mouse inputs if the mouse is within the client area 
    POINT currentCursorPosition{};
    RECT clientRect;
    if (GetCursorPos(&currentCursorPosition) == FALSE ||
        ScreenToClient(windowHandle, &currentCursorPosition) == FALSE ||
        GetClientRect(windowHandle, &clientRect) == FALSE ||
        currentCursorPosition.x < clientRect.left ||
        currentCursorPosition.x > clientRect.right ||
        currentCursorPosition.y < clientRect.top ||
        currentCursorPosition.y > clientRect.bottom
    ) {
        return 0;
    }

    processWin32MouseMovedInput(windowHandle, rawInput);
    processWin32MouseDownInput(rawInput);
    processWin32MouseUpInput(rawInput);
    processWin32MouseScrollInput(rawInput);

    wasHandled = true;
    return 0;
}

void AxrIOActionSystem::processWin32MouseMovedInput(const HWND windowHandle, const RAWINPUT* rawInput) {
    // If the mouse device itself sends absolute movement data
    // NOTE: This has never been tested. Need a mouse that uses absolute positioning
    if (rawInput->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
        RECT rect;
        if (rawInput->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) {
            rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
            rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
            rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
            rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        } else {
            rect.left = 0;
            rect.top = 0;
            rect.right = GetSystemMetrics(SM_CXSCREEN);
            rect.bottom = GetSystemMetrics(SM_CYSCREEN);
        }

        // These values should be the absolute values in screen space.
        const POINT absolutePosition{
            .x = MulDiv(rawInput->data.mouse.lLastX, rect.right, USHRT_MAX) + rect.left,
            .y = MulDiv(rawInput->data.mouse.lLastY, rect.bottom, USHRT_MAX) + rect.top
        };

        // Convert to client space
        POINT cursorPosition{
            .x = absolutePosition.x,
            .y = absolutePosition.y
        };
        ScreenToClient(windowHandle, &cursorPosition);

        triggerVec2InputAction(
            AXR_VEC2_INPUT_ACTION_MOUSE_MOVED,
            AxrVec2{
                .x = m_LastAbsoluteCursorPosition.x - static_cast<float>(absolutePosition.x),
                .y = m_LastAbsoluteCursorPosition.y - static_cast<float>(absolutePosition.y)
            }
        );

        m_LastAbsoluteCursorPosition = AxrVec2{
            .x = static_cast<float>(absolutePosition.x),
            .y = static_cast<float>(absolutePosition.y)
        };

        triggerVec2InputAction(
            AXR_VEC2_INPUT_ACTION_MOUSE_POSITION,
            AxrVec2{
                .x = static_cast<float>(cursorPosition.x),
                .y = static_cast<float>(cursorPosition.y)
            }
        );

        // TODO: Add ability to lock the cursor to the center of the screen
        // resetLockedCursorPosition(WindowHandle);
    }

    // If the mouse device itself sends relative movement data
    else if (rawInput->data.mouse.lLastX != 0 || rawInput->data.mouse.lLastY != 0) {
        const int relativeX = rawInput->data.mouse.lLastX;
        const int relativeY = rawInput->data.mouse.lLastY;

        triggerVec2InputAction(
            AXR_VEC2_INPUT_ACTION_MOUSE_MOVED,
            AxrVec2{
                .x = static_cast<float>(relativeX),
                .y = static_cast<float>(relativeY)
            }
        );

        POINT cursorPosition{};
        if (GetCursorPos(&cursorPosition)) {
            ScreenToClient(windowHandle, &cursorPosition);

            triggerVec2InputAction(
                AXR_VEC2_INPUT_ACTION_MOUSE_POSITION,
                AxrVec2{
                    .x = static_cast<float>(cursorPosition.x),
                    .y = static_cast<float>(cursorPosition.y)
                }
            );
        }

        // TODO: Add ability to lock the cursor to the center of the screen
        // resetLockedCursorPosition(WindowHandle);
    }
}

void AxrIOActionSystem::processWin32MouseDownInput(const RAWINPUT* rawInput) {
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
        const auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_MouseClickLStartTime
        );

        if (timeSinceLastClick >= std::chrono::milliseconds{m_DoubleClickTime}) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L, true);
            m_MouseClickLStartTime = std::chrono::high_resolution_clock::now();
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L, true);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
        const auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_MouseClickRStartTime
        );

        if (timeSinceLastClick >= std::chrono::milliseconds{m_DoubleClickTime}) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R, true);
            m_MouseClickRStartTime = std::chrono::high_resolution_clock::now();
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R, true);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
        const auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_MouseClickMStartTime
        );

        if (timeSinceLastClick >= std::chrono::milliseconds{m_DoubleClickTime}) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M, true);
            m_MouseClickMStartTime = std::chrono::high_resolution_clock::now();
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M, true);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
        const auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_MouseClickX1StartTime
        );

        if (timeSinceLastClick >= std::chrono::milliseconds{m_DoubleClickTime}) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1, true);
            m_MouseClickX1StartTime = std::chrono::high_resolution_clock::now();
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1, true);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
        const auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_MouseClickX2StartTime
        );

        if (timeSinceLastClick >= std::chrono::milliseconds{m_DoubleClickTime}) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2, true);
            m_MouseClickX2StartTime = std::chrono::high_resolution_clock::now();
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2, true);
        }
    }
}

void AxrIOActionSystem::processWin32MouseUpInput(const RAWINPUT* rawInput) {
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
        // If the left mouse button was doubled clicked, trigger the double-clicked up event.
        // Otherwise, trigger the normal mouse up event
        if (m_ActiveBoolInputActions.contains(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L)) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_L, false);
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_L, false);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
        // If the right mouse button was doubled clicked, trigger the double-clicked up event.
        // Otherwise, trigger the normal mouse up event
        if (m_ActiveBoolInputActions.contains(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R)) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_R, false);
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_R, false);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
        // If the middle mouse button was doubled clicked, trigger the double-clicked up event.
        // Otherwise, trigger the normal mouse up event
        if (m_ActiveBoolInputActions.contains(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M)) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_M, false);
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_M, false);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
        // If the X1 mouse button was doubled clicked, trigger the double-clicked up event.
        // Otherwise, trigger the normal mouse up event
        if (m_ActiveBoolInputActions.contains(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1)) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X1, false);
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X1, false);
        }
    }
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
        // If the X2 mouse button was doubled clicked, trigger the double-clicked up event.
        // Otherwise, trigger the normal mouse up event
        if (m_ActiveBoolInputActions.contains(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2)) {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_DOUBLE_CLICK_X2, false);
        } else {
            triggerBoolInputAction(AXR_BOOL_INPUT_ACTION_MOUSE_CLICK_X2, false);
        }
    }
}

void AxrIOActionSystem::processWin32MouseScrollInput(const RAWINPUT* rawInput) {
    // Vertical Scroll Wheel
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
        const auto wheelDelta = static_cast<short>(rawInput->data.mouse.usButtonData);
        const float scrollDelta = static_cast<float>(wheelDelta) / WHEEL_DELTA;

        triggerFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL, scrollDelta);
    }

    // Horizontal Scroll Wheel
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_HWHEEL) {
        const auto wheelDelta = static_cast<short>(rawInput->data.mouse.usButtonData);
        const float scrollDelta = static_cast<float>(wheelDelta) / WHEEL_DELTA;

        triggerFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL, scrollDelta);
    }
}
#endif
