#ifdef AXR_USE_PLATFORM_WIN32

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "win32WindowSystem.hpp"
#include "axr/logger.h"
#include "../../utils.hpp"

// ---- Special Functions ----

AxrWin32WindowSystem::AxrWin32WindowSystem(const Config& config) :
    m_ApplicationName(config.ApplicationName),
    m_ActionSystem(config.ActionSystem),
    m_Width(config.Width),
    m_Height(config.Height),
    m_OnWindowResizedCallback(config.OnWindowResizedCallback),
    m_WindowClassName(axrToWString(config.ApplicationName + "_Class")),
    m_Instance(nullptr),
    m_WindowHandle(nullptr),
    m_IsCursorLocked(false),
    m_IsCursorHidden(false),
    m_UnlockedCursorClipRect(
        RECT{
            .left = 0,
            .top = 0,
            .right = 0,
            .bottom = 0,
        }
    ) {
}

AxrWin32WindowSystem::~AxrWin32WindowSystem() {
    destroyWin32Window();
}

// ---- Public Functions ----

AxrResult AxrWin32WindowSystem::setup() {
    m_Instance = GetModuleHandle(nullptr);

    // Check if window class is already registered
    WNDCLASSEX windowClassInfo;
    if (GetClassInfoEx(m_Instance, m_WindowClassName.c_str(), &windowClassInfo) != 0) {
        axrLogErrorLocation("Window class is already registered.");
        return AXR_ERROR;
    }

    // ---- Create Window Class ----
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc = processWindowMessage;
    windowClass.hInstance = m_Instance;
    windowClass.lpszClassName = m_WindowClassName.c_str();
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (RegisterClassEx(&windowClass) == 0) {
        axrLogErrorLocation("Failed to register class.");
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

bool AxrWin32WindowSystem::isWindowOpen() const {
    return m_WindowHandle != nullptr;
}

AxrResult AxrWin32WindowSystem::openWindow() {
    if (isWindowOpen()) {
        axrLogWarningLocation("Window already open.");
        return AXR_SUCCESS;
    }

    const HINSTANCE hInstance = GetModuleHandle(nullptr);
    const std::wstring windowName = axrToWString(m_ApplicationName);

    m_WindowHandle = CreateWindowEx(
        0,
        m_WindowClassName.c_str(),
        windowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        // Window Position
        CW_USEDEFAULT,
        CW_USEDEFAULT,

        // Window Size
        static_cast<int>(m_Width),
        static_cast<int>(m_Height),

        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (m_WindowHandle == nullptr) {
        axrLogErrorLocation("Failed to create window handle.");
        return AXR_ERROR;
    }

    ShowWindow(m_WindowHandle, SW_SHOW);

    // Gives us access to this class in the static processWindowMessage() function
    SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    return AXR_SUCCESS;
}

void AxrWin32WindowSystem::closeWindow() {
    if (!isWindowOpen()) return;

    PostMessage(m_WindowHandle, WM_CLOSE, 0, 0);
}

bool AxrWin32WindowSystem::isCursorHidden() const {
    return m_IsCursorHidden;
}

void AxrWin32WindowSystem::showCursor() {
    CURSORINFO cursorInfo{
        .cbSize = sizeof(CURSORINFO),
    };

    if (GetCursorInfo(&cursorInfo) == FALSE) {
        axrLogErrorLocation("Failed to get cursor info. Error: {0}.", GetLastError());
        return;
    }

    // If the cursor is hidden
    if (cursorInfo.flags == 0) {
        // Continue to show the cursor until it's count returns 0 or more
        // For more info, visit https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showcursor#remarks
        while (ShowCursor(TRUE) < 0);
    }

    m_IsCursorHidden = false;
}

void AxrWin32WindowSystem::hideCursor() {
    CURSORINFO cursorInfo{
        .cbSize = sizeof(CURSORINFO),
    };

    if (GetCursorInfo(&cursorInfo) == FALSE) {
        axrLogErrorLocation("Failed to get cursor info. Error: {0}.", GetLastError());
        return;
    }

    // If the cursor is visible
    if (cursorInfo.flags == CURSOR_SHOWING) {
        // Continue to hide the cursor until it's count returns less than 0
        // For more info, visit https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showcursor#remarks
        while (ShowCursor(FALSE) >= 0);
    }

    m_IsCursorHidden = true;
}

bool AxrWin32WindowSystem::isCursorLocked() const {
    return m_IsCursorLocked;
}

void AxrWin32WindowSystem::lockCursor() {
    CURSORINFO cursorInfo{
        .cbSize = sizeof(CURSORINFO),
    };

    if (GetCursorInfo(&cursorInfo) == FALSE) {
        axrLogErrorLocation("Failed to get cursor info. Error: {0}.", GetLastError());
        return;
    }

    if (GetClipCursor(&m_UnlockedCursorClipRect) == FALSE) {
        axrLogErrorLocation("Failed to get clip cursor rect. Error: {0}.", GetLastError());
        return;
    }

    const RECT windowRect{
        .left = cursorInfo.ptScreenPos.x,
        .top = cursorInfo.ptScreenPos.y,
        .right = cursorInfo.ptScreenPos.x,
        .bottom = cursorInfo.ptScreenPos.y,
    };

    if (ClipCursor(&windowRect) == FALSE) {
        axrLogErrorLocation("Failed to restrict the cursor's position. Error: {0}.", GetLastError());
        return;
    }

    m_IsCursorLocked = true;
}

void AxrWin32WindowSystem::unlockCursor() {
    if (ClipCursor(&m_UnlockedCursorClipRect) == FALSE) {
        axrLogErrorLocation("Failed to restrict the cursor's position. Error: {0}.", GetLastError());
        return;
    }

    m_IsCursorLocked = false;
}

bool AxrWin32WindowSystem::processEvents() {
    if (!isWindowOpen()) return true;

    MSG message{};
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            destroyWin32Window();
            return false;
        }

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return true;
}

AxrResult AxrWin32WindowSystem::getClientSize(uint32_t& width, uint32_t& height) const {
    RECT rect;
    if (GetClientRect(m_WindowHandle, &rect)) {
        width = static_cast<uint32_t>(rect.right - rect.left);
        height = static_cast<uint32_t>(rect.bottom - rect.top);
        return AXR_SUCCESS;
    }

    axrLogErrorLocation("Failed to retrieve client size.");
    return AXR_ERROR;
}

HINSTANCE AxrWin32WindowSystem::getInstance() const {
    return m_Instance;
}

HWND AxrWin32WindowSystem::getWindowHandle() const {
    return m_WindowHandle;
}

// ---- Private Functions ----

void AxrWin32WindowSystem::destroyWin32Window() {
    if (m_WindowHandle == nullptr) return;

    DestroyWindow(m_WindowHandle);
    m_WindowHandle = nullptr;
}

LRESULT AxrWin32WindowSystem::processWindowMessageInternal(
    const HWND windowHandle,
    const UINT uMsg,
    const WPARAM wParam,
    const LPARAM lParam,
    bool& wasHandled
) {
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT: {
            PostQuitMessage(0);
            wasHandled = true;
            return 0;
        }
        case WM_SIZE: {
            m_OnWindowResizedCallback(LOWORD(lParam), HIWORD(lParam));
            wasHandled = true;
            return 0;
        }
        default: {
            return 0;
        }
    }
}

// ---- Private Static Functions ----

LRESULT AxrWin32WindowSystem::processWindowMessage(
    const HWND windowHandle,
    const UINT uMsg,
    const WPARAM wParam,
    const LPARAM lParam
) {
    // NOLINTNEXTLINE(performance-no-int-to-ptr)
    AxrWin32WindowSystem* windowSystemHandle = reinterpret_cast<AxrWin32WindowSystem*>(GetWindowLongPtr(
        windowHandle,
        GWLP_USERDATA
    ));

    if (windowSystemHandle == nullptr) {
        return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }


    if (windowSystemHandle->isWindowOpen()) {
        bool wasHandled = false;

        LRESULT processEventResult = windowSystemHandle->processWindowMessageInternal(
            windowHandle,
            uMsg,
            wParam,
            lParam,
            wasHandled
        );

        if (wasHandled) return processEventResult;

        processEventResult = windowSystemHandle->m_ActionSystem->processWin32Message(
            windowHandle,
            uMsg,
            wParam,
            lParam,
            wasHandled
        );

        if (wasHandled) return processEventResult;
    }

    return DefWindowProc(windowHandle, uMsg, wParam, lParam);
}

#endif
