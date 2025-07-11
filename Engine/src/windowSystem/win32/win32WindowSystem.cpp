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
    m_IOActionSystem(config.IOActionSystem),
    m_Width(config.Width),
    m_Height(config.Height),
    m_OnWindowResizedCallback(config.OnWindowResizedCallback),
    m_WindowClassName(axrToWString(config.ApplicationName + "_Class")),
    m_Instance(nullptr),
    m_WindowHandle(nullptr) {
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

        processEventResult = windowSystemHandle->m_IOActionSystem->processWin32Message(
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
