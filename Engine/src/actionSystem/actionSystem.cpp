// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "actionSystem.hpp"
#include "axr/logger.h"
#include "actionUtils.hpp"

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

AxrActionSet_T axrActionSystemGetActionSet(const AxrActionSystem_T actionSystem, const char* name) {
    if (actionSystem == nullptr) {
        axrLogErrorLocation("`actionSystem` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSystem->getActionSet(name);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrActionSystem::AxrActionSystem(const Config& config):
    m_XrSystem(config.XrSystem),
    m_DoubleClickTime(0),
    m_LastAbsoluteCursorPosition(
        AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        }
    ),
    m_ScrollDelta(0.0f),
    m_HorizontalScrollDelta(0.0f),
    m_MouseMovedDelta(
        AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        }
    ),
    m_AreXrActionsAttached(false) {
    if (config.ActionSets != nullptr) {
        for (uint32_t i = 0; i < config.ActionSetCount; ++i) {
            m_ActionSets.insert(
                std::pair(
                    config.ActionSets[i].Name,
                    AxrActionSet(
                        AxrActionSet::Config{
                            .Name = config.ActionSets[i].Name,
                            .LocalizedName = config.ActionSets[i].LocalizedName,
                            .BoolInputActionCount = config.ActionSets[i].BoolInputActionCount,
                            .BoolInputActions = config.ActionSets[i].BoolInputActions,
                            .FloatInputActionCount = config.ActionSets[i].FloatInputActionCount,
                            .FloatInputActions = config.ActionSets[i].FloatInputActions,
                            .Vec2InputActionCount = config.ActionSets[i].Vec2InputActionCount,
                            .Vec2InputActions = config.ActionSets[i].Vec2InputActions,
                            .PoseInputActionCount = config.ActionSets[i].PoseInputActionCount,
                            .PoseInputActions = config.ActionSets[i].PoseInputActions,
                        }
                    )
                )
            );
        }
        for (uint32_t i = 0; i < config.XrInteractionProfileCount; ++i) {
            m_XrInteractionProfiles.insert(config.XrInteractionProfiles[i]);
        }
    }
}

AxrActionSystem::~AxrActionSystem() {
    resetSetup();
}

// ---- Public Functions ----

AxrActionSet_T AxrActionSystem::getActionSet(const std::string& name) {
    const auto foundActionSet = m_ActionSets.find(name);
    if (foundActionSet == m_ActionSets.end()) {
        return nullptr;
    }

    return &foundActionSet->second;
}

AxrResult AxrActionSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

#ifdef AXR_USE_PLATFORM_WIN32
    axrResult = setupWin32Inputs();
    if (AXR_FAILED(axrResult)) return axrResult;
#endif

    axrResult = setupXrActions();
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}

void AxrActionSystem::resetSetup() {
    clearInputActions();

    resetSetupXrActions();
#ifdef AXR_USE_PLATFORM_WIN32
    resetSetupWin32Inputs();
#endif
}

void AxrActionSystem::newFrameStarted() {
    resetRelativeActions();

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        actionSet.newFrameStarted();
    }
}

void AxrActionSystem::processEvents() {
    triggerRelativeActions();
}

#ifdef AXR_USE_PLATFORM_WIN32
// ReSharper disable once CppDFAConstantFunctionResult
LRESULT AxrActionSystem::processWin32Message(
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
        case WM_KEYDOWN: {
            const LRESULT result = processWin32KeyDown(wParam, wasHandled);
            if (wasHandled) {
                return result;
            }
            break;
        }
        case WM_KEYUP: {
            const LRESULT result = processWin32KeyUp(wParam, wasHandled);
            if (wasHandled) {
                return result;
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

void AxrActionSystem::triggerBoolInputAction(const AxrBoolInputActionEnum inputActionEnum, const bool value) {
    if (value) {
        m_ActiveBoolInputActions.insert(inputActionEnum);
    } else {
        m_ActiveBoolInputActions.erase(inputActionEnum);
    }

    uint32_t highestPriority = 0;
    std::vector<AxrActionSet*> actionSetsToTrigger;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (!actionSet.containsBinding(inputActionEnum)) continue;

        // Only trigger actions on sets of the highest priority
        const uint32_t priority = actionSet.getPriority();
        if (priority > highestPriority) {
            actionSetsToTrigger.clear();
            highestPriority = priority;
        } else if (priority < highestPriority) continue;

        actionSetsToTrigger.push_back(&actionSet);
    }

    for (AxrActionSet* actionSet : actionSetsToTrigger) {
        actionSet->triggerBoolInputAction(inputActionEnum, value);
    }
}

void AxrActionSystem::triggerFloatInputAction(const AxrFloatInputActionEnum inputActionEnum, const float value) {
    uint32_t highestPriority = 0;
    std::vector<AxrActionSet*> actionSetsToTrigger;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (!actionSet.containsBinding(inputActionEnum)) continue;

        // Only trigger actions on sets of the highest priority
        const uint32_t priority = actionSet.getPriority();
        if (priority > highestPriority) {
            actionSetsToTrigger.clear();
            highestPriority = priority;
        } else if (priority < highestPriority) continue;

        actionSetsToTrigger.push_back(&actionSet);
    }

    for (AxrActionSet* actionSet : actionSetsToTrigger) {
        actionSet->triggerFloatInputAction(inputActionEnum, value);
    }
}

void AxrActionSystem::triggerVec2InputAction(const AxrVec2InputActionEnum inputActionEnum, const AxrVec2& value) {
    uint32_t highestPriority = 0;
    std::vector<AxrActionSet*> actionSetsToTrigger;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (!actionSet.containsBinding(inputActionEnum)) continue;

        // Only trigger actions on sets of the highest priority
        const uint32_t priority = actionSet.getPriority();
        if (priority > highestPriority) {
            actionSetsToTrigger.clear();
            highestPriority = priority;
        } else if (priority < highestPriority) continue;

        actionSetsToTrigger.push_back(&actionSet);
    }

    for (AxrActionSet* actionSet : actionSetsToTrigger) {
        actionSet->triggerVec2InputAction(inputActionEnum, value);
    }
}

void AxrActionSystem::triggerPoseInputAction(const AxrPoseInputActionEnum inputActionEnum, const AxrPose& value) {
    uint32_t highestPriority = 0;
    std::vector<AxrActionSet*> actionSetsToTrigger;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        if (!actionSet.containsBinding(inputActionEnum)) continue;

        // Only trigger actions on sets of the highest priority
        const uint32_t priority = actionSet.getPriority();
        if (priority > highestPriority) {
            actionSetsToTrigger.clear();
            highestPriority = priority;
        } else if (priority < highestPriority) continue;

        actionSetsToTrigger.push_back(&actionSet);
    }

    for (AxrActionSet* actionSet : actionSetsToTrigger) {
        actionSet->triggerPoseInputAction(inputActionEnum, value);
    }
}

void AxrActionSystem::resetBoolInputAction(const AxrBoolInputActionEnum inputActionEnum) {
    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        for (AxrBoolInputAction& inputAction : actionSet.getBoolInputActions() | std::ranges::views::values) {
            if (inputAction.containsBinding(inputActionEnum)) {
                inputAction.reset();
            }
        }
    }
}

void AxrActionSystem::resetFloatInputAction(const AxrFloatInputActionEnum inputActionEnum) {
    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        for (AxrFloatInputAction& inputAction : actionSet.getFloatInputActions() | std::ranges::views::values) {
            if (inputAction.containsBinding(inputActionEnum)) {
                inputAction.reset();
            }
        }
    }
}

void AxrActionSystem::resetVec2InputAction(const AxrVec2InputActionEnum inputActionEnum) {
    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        for (AxrVec2InputAction& inputAction : actionSet.getVec2InputActions() | std::ranges::views::values) {
            if (inputAction.containsBinding(inputActionEnum)) {
                inputAction.reset();
            }
        }
    }
}

void AxrActionSystem::resetPoseInputAction(const AxrPoseInputActionEnum inputActionEnum) {
    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        for (AxrPoseInputAction& inputAction : actionSet.getPoseInputActions() | std::ranges::views::values) {
            if (inputAction.getBinding() == inputActionEnum) {
                inputAction.reset();
            }
        }
    }
}

void AxrActionSystem::triggerRelativeActions() {
    if (m_ScrollDelta != 0.0f) {
        triggerFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL, m_ScrollDelta);
    }
    if (m_HorizontalScrollDelta != 0.0f) {
        triggerFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL, m_HorizontalScrollDelta);
    }
    if (m_MouseMovedDelta.x != 0.0f || m_MouseMovedDelta.y != 0.0f) {
        triggerVec2InputAction(AXR_VEC2_INPUT_ACTION_MOUSE_MOVED, m_MouseMovedDelta);
    }
}

void AxrActionSystem::resetRelativeActions() {
    m_ScrollDelta = 0.0f;
    m_HorizontalScrollDelta = 0.0f;
    m_MouseMovedDelta = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };

    resetFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL);
    resetFloatInputAction(AXR_FLOAT_INPUT_ACTION_MOUSE_WHEEL_HORIZONTAL);
    resetVec2InputAction(AXR_VEC2_INPUT_ACTION_MOUSE_MOVED);
}

void AxrActionSystem::clearInputActions() {
    m_ActiveBoolInputActions.clear();
    m_MouseClickLStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickRStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickMStartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickX1StartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_MouseClickX2StartTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    m_LastAbsoluteCursorPosition = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };

    m_ScrollDelta = 0.0f;
    m_HorizontalScrollDelta = 0.0f;
    m_MouseMovedDelta = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };
}

AxrResult AxrActionSystem::setupXrActions() {
    if (m_XrSystem == nullptr) return AXR_SUCCESS;

    AxrResult axrResult = AXR_SUCCESS;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        axrResult = actionSet.setupXrActions(m_XrSystem);
        if (AXR_FAILED(axrResult)) {
            resetSetupXrActions();
            return axrResult;
        }
    }

    axrResult = suggestXrBindings();
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }

    m_XrSystem->OnXrSessionStateChangedCallbackActions
              .connect<&AxrActionSystem::onXrSessionStateChangedCallback>(this);

    return AXR_SUCCESS;
}

void AxrActionSystem::resetSetupXrActions() {
    if (m_XrSystem == nullptr) {
        axrLogWarningLocation("Xr system is null.");
        return;
    }

    m_XrSystem->OnXrSessionStateChangedCallbackActions.reset();
    resetSetupXrSessionData();

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        actionSet.resetSetupXrActions();
    }
}

AxrResult AxrActionSystem::setupXrSessionData() {
    if (m_XrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null.");
        return AXR_ERROR;
    }

    AxrResult axrResult = createXrSpaces();
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }

    axrResult = m_XrSystem->attachActionSets(m_ActionSets);
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }
    m_AreXrActionsAttached = true;

    return AXR_SUCCESS;
}

void AxrActionSystem::resetSetupXrSessionData() {
    m_AreXrActionsAttached = false;
    if (m_XrSystem != nullptr) {
        m_XrSystem->detachActionSets();
    }

    destroyXrSpaces();
}

AxrResult AxrActionSystem::suggestXrBindings() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_XrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::vector<AxrXrSystem::ActionBinding> actionBindings;

    for (AxrActionSet& xrActionSet : m_ActionSets | std::ranges::views::values) {
        for (AxrBoolInputAction& inputAction : xrActionSet.getBoolInputActions() | std::ranges::views::values) {
            for (const AxrBoolInputActionEnum inputActionEnum : inputAction.getBindings()) {
                const XrAction action = inputAction.getXrAction();

                if (axrIsXrBoolInputAction(inputActionEnum) && action != XR_NULL_HANDLE) {
                    actionBindings.push_back(
                        AxrXrSystem::ActionBinding{
                            .Action = action,
                            .bindingName = axrGetXrBoolInputActionName(inputActionEnum),
                        }
                    );
                }
            }
        }

        for (AxrFloatInputAction& inputAction : xrActionSet.getFloatInputActions() | std::ranges::views::values) {
            for (const AxrFloatInputActionEnum inputActionEnum : inputAction.getBindings()) {
                const XrAction action = inputAction.getXrAction();

                if (axrIsXrFloatInputAction(inputActionEnum) && action != XR_NULL_HANDLE) {
                    actionBindings.push_back(
                        AxrXrSystem::ActionBinding{
                            .Action = action,
                            .bindingName = axrGetXrFloatInputActionName(inputActionEnum),
                        }
                    );
                }
            }
        }

        for (AxrVec2InputAction& inputAction : xrActionSet.getVec2InputActions() | std::ranges::views::values) {
            for (const AxrVec2InputActionEnum inputActionEnum : inputAction.getBindings()) {
                const XrAction action = inputAction.getXrAction();

                if (axrIsXrVec2InputAction(inputActionEnum) && action != XR_NULL_HANDLE) {
                    actionBindings.push_back(
                        AxrXrSystem::ActionBinding{
                            .Action = action,
                            .bindingName = axrGetXrVec2InputActionName(inputActionEnum),
                        }
                    );
                }
            }
        }

        for (AxrPoseInputAction& inputAction : xrActionSet.getPoseInputActions() | std::ranges::views::values) {
            const AxrPoseInputActionEnum inputActionEnum = inputAction.getBinding();

            const XrAction action = inputAction.getXrAction();

            if (axrIsXrPoseInputAction(inputActionEnum) && action != XR_NULL_HANDLE) {
                actionBindings.push_back(
                    AxrXrSystem::ActionBinding{
                        .Action = action,
                        .bindingName = axrGetXrPoseInputActionName(inputActionEnum),
                    }
                );
            }
        }
    }

    for (const AxrXrInteractionProfileEnum xrInteractionProfile : m_XrInteractionProfiles) {
        const AxrResult axrResult = m_XrSystem->suggestBindings(xrInteractionProfile, actionBindings);
        if (AXR_FAILED(axrResult)) return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrActionSystem::onXrSessionStateChangedCallback(const bool isSessionRunning) {
    if (isSessionRunning) {
        return setupXrSessionData();
    }

    resetSetupXrSessionData();
    return AXR_SUCCESS;
}

AxrResult AxrActionSystem::createXrSpaces() {
    AxrResult axrResult = AXR_SUCCESS;

    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        axrResult = actionSet.createXrSpaces();
        if (AXR_FAILED(axrResult)) {
            destroyXrSpaces();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrActionSystem::destroyXrSpaces() {
    for (AxrActionSet& actionSet : m_ActionSets | std::ranges::views::values) {
        actionSet.destroyXrSpaces();
    }
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrResult AxrActionSystem::setupWin32Inputs() {
    const AxrResult axrResult = registerWin32RawInputs();
    if (AXR_FAILED(axrResult)) return axrResult;

    m_DoubleClickTime = GetDoubleClickTime();

    return AXR_SUCCESS;
}

void AxrActionSystem::resetSetupWin32Inputs() {
    m_DoubleClickTime = 0;
}

AxrResult AxrActionSystem::registerWin32RawInputs() const {
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
LRESULT AxrActionSystem::processWin32MouseInput(const HWND windowHandle, const RAWINPUT* rawInput, bool& wasHandled) {
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

void AxrActionSystem::processWin32MouseMovedInput(const HWND windowHandle, const RAWINPUT* rawInput) {
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

        m_MouseMovedDelta.x += m_LastAbsoluteCursorPosition.x - static_cast<float>(absolutePosition.x);
        m_MouseMovedDelta.y += m_LastAbsoluteCursorPosition.y - static_cast<float>(absolutePosition.y);

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

        m_MouseMovedDelta.x += static_cast<float>(relativeX);
        m_MouseMovedDelta.y += static_cast<float>(relativeY);

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

void AxrActionSystem::processWin32MouseDownInput(const RAWINPUT* rawInput) {
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

void AxrActionSystem::processWin32MouseUpInput(const RAWINPUT* rawInput) {
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

void AxrActionSystem::processWin32MouseScrollInput(const RAWINPUT* rawInput) {
    // Vertical Scroll Wheel
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
        const auto wheelDelta = static_cast<short>(rawInput->data.mouse.usButtonData);
        const float scrollDelta = static_cast<float>(wheelDelta) / WHEEL_DELTA;

        m_ScrollDelta += scrollDelta;
    }

    // Horizontal Scroll Wheel
    if (rawInput->data.mouse.usButtonFlags & RI_MOUSE_HWHEEL) {
        const auto wheelDelta = static_cast<short>(rawInput->data.mouse.usButtonData);
        const float scrollDelta = static_cast<float>(wheelDelta) / WHEEL_DELTA;

        m_HorizontalScrollDelta += scrollDelta;
    }
}

LRESULT AxrActionSystem::processWin32KeyDown(const WPARAM wParam, bool& wasHandled) {
    triggerBoolInputAction(
        axrWParamToBoolInputActionEnum(wParam),
        true
    );

    wasHandled = true;
    return 0;
}

LRESULT AxrActionSystem::processWin32KeyUp(const WPARAM wParam, bool& wasHandled) {
    triggerBoolInputAction(
        axrWParamToBoolInputActionEnum(wParam),
        false
    );

    wasHandled = true;
    return 0;
}
#endif
