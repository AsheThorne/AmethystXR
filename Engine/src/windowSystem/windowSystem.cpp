// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "windowSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrWindowSystemIsWindowOpen(const AxrWindowSystem_T windowSystem) {
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

void axrWindowSystemProcessEvents(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogErrorLocation("`windowSystem` is null.");
        return;
    }

    windowSystem->processEvents();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrWindowSystem::AxrWindowSystem(const Config& config) {
#ifdef AXR_USE_PLATFORM_WIN32
    m_Win32WindowSystem = new AxrWin32WindowSystem(
        AxrWin32WindowSystem::Config{
            .ApplicationName = config.ApplicationName,
            .Width = config.WindowConfig.Width,
            .Height = config.WindowConfig.Height,
            .OnWindowResizedCallback = AxrCallback(this, onWindowResizedCallback)
        }
    );
#endif
}

AxrWindowSystem::~AxrWindowSystem() {
    cleanup();
}

// ---- Public Functions ----

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

void AxrWindowSystem::cleanup() {
#ifdef AXR_USE_PLATFORM_WIN32
    if (m_Win32WindowSystem != nullptr) {
        delete m_Win32WindowSystem;
        m_Win32WindowSystem = nullptr;
    }
#endif

    OnWindowOpenStateChangedCallbackGraphics = {};
    OnWindowResizedCallbackGraphics = {};
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

void AxrWindowSystem::invokeOnWindowResizedCallbacks(const uint32_t width, const uint32_t height) const {
    OnWindowResizedCallbackGraphics(width, height);
}

// ---- Private Static Functions ----

void AxrWindowSystem::onWindowResizedCallback(void* userData, const uint32_t width, const uint32_t height) {
    if (userData == nullptr) {
        axrLogErrorLocation("userData is null.");
        return;
    }

    auto self = reinterpret_cast<AxrWindowSystem_T>(userData);
    self->invokeOnWindowResizedCallbacks(width, height);
}
