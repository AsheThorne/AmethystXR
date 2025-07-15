// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "windowSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrWindowSystemIsValid(const AxrWindowSystemConst_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogErrorLocation("`windowSystem` is null.");
        return false;
    }

    return windowSystem->isValid();
}

bool axrWindowSystemIsWindowOpen(const AxrWindowSystemConst_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogErrorLocation("`windowSystem` is null.");
        return false;
    }

    return windowSystem->isWindowOpen();
}

AxrResult axrWindowSystemOpenWindow(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogErrorLocation("`windowSystem` is null.");
        return AXR_ERROR;
    }

    return windowSystem->openWindow();
}

void axrWindowSystemCloseWindow(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogErrorLocation("`windowSystem` is null.");
        return;
    }

    windowSystem->closeWindow();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrWindowSystem::AxrWindowSystem(std::nullptr_t) {
}

AxrWindowSystem::AxrWindowSystem(const Config& config):
    m_IsValid(true) {
#ifdef AXR_USE_PLATFORM_WIN32
    OnWindowResizedCallback_T windowResizedCallback;
    windowResizedCallback.connect<&AxrWindowSystem::onWindowResizedCallback>(this);

    m_Win32WindowSystem = new AxrWin32WindowSystem(
        AxrWin32WindowSystem::Config{
            .ApplicationName = config.ApplicationName,
            .ActionSystem = config.ActionSystem,
            .Width = config.Width,
            .Height = config.Height,
            .OnWindowResizedCallback = windowResizedCallback
        }
    );
#endif
}

AxrWindowSystem::~AxrWindowSystem() {
    resetSetup();
}

// ---- Public Functions ----

bool AxrWindowSystem::isValid() const {
    return m_IsValid;
}

bool AxrWindowSystem::isWindowOpen() const {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return false;
    }

    return m_Win32WindowSystem->isWindowOpen();
#else
    axrLogErrorLocation("Unknown platform.");
    return false;
#endif
}

AxrResult AxrWindowSystem::openWindow() {
    AxrResult axrResult = AXR_ERROR;

#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
    } else {
        axrResult = m_Win32WindowSystem->openWindow();
    }
#else
    axrLogErrorLocation("Unknown platform.");
#endif

    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    axrResult = OnWindowOpenStateChangedCallbackGraphics(true);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrWindowSystem::closeWindow() {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    m_Win32WindowSystem->closeWindow();
#else
    axrLogErrorLocation("Unknown platform.");
    return;
#endif

    if (AXR_FAILED(OnWindowOpenStateChangedCallbackGraphics(false))) {
        axrLogErrorLocation("Failed to clean up window graphics.");
    }
}

AxrResult AxrWindowSystem::setup() {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->setup();
#else
    axrLogErrorLocation("Unknown platform.");
    return AXR_ERROR;
#endif
}

void AxrWindowSystem::resetSetup() {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem != nullptr) {
        delete m_Win32WindowSystem;
        m_Win32WindowSystem = nullptr;
    }
#endif

    OnWindowOpenStateChangedCallbackGraphics.reset();
    OnWindowResizedCallbackGraphics.reset();
}

void AxrWindowSystem::processEvents() {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    if (!m_Win32WindowSystem->processEvents()) {
        // If the window closed, signal the window graphics
        if (AXR_FAILED(OnWindowOpenStateChangedCallbackGraphics(false))) {
            axrLogErrorLocation("Failed to clean up window graphics.");
        }
    }
#else
    axrLogErrorLocation("Unknown platform.");
#endif
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrWin32WindowSystem* AxrWindowSystem::getWin32WindowSystem() const {
    return m_Win32WindowSystem;
}
#endif

AxrResult AxrWindowSystem::getClientSize(uint32_t& width, uint32_t& height) const {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->getClientSize(width, height);
#else
    axrLogErrorLocation("Unknown platform.");
    return AXR_ERROR;
#endif
}

// ---- Private Functions ----

void AxrWindowSystem::onWindowResizedCallback(const uint32_t width, const uint32_t height) const {
    OnWindowResizedCallbackGraphics(width, height);
}
