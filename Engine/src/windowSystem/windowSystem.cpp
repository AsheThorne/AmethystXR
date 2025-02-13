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

AxrWindowSystem::AxrWindowSystem(const Config& config):
    m_ConfigureWindowGraphicsCallbackUserData(nullptr),
    m_ConfigureWindowGraphicsCallback(nullptr) {
#ifdef AXR_USE_PLATFORM_WIN32
    m_Win32WindowSystem = std::make_unique<AxrWin32WindowSystem>(
        AxrWin32WindowSystem::Config{
            .ApplicationName = config.ApplicationName,
            .Width = config.WindowConfig.Width,
            .Height = config.WindowConfig.Height,
        }
    );
#endif
}

// ---- Public Functions ----

bool AxrWindowSystem::isWindowOpen() const {
#ifdef AXR_USE_PLATFORM_WIN32
    return isWin32WindowOpen();
#else
    axrLogErrorLocation("Unknown platform.");
    return false;
#endif
}

AxrResult AxrWindowSystem::openWindow() {
    AxrResult axrResult = AXR_ERROR;

#ifdef AXR_USE_PLATFORM_WIN32
    axrResult = openWin32Window();
#else
    axrLogErrorLocation("Unknown platform.");
#endif

    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    axrResult = invokeConfigureWindowGraphicsCallback(true);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return axrResult;
}

void AxrWindowSystem::closeWindow() {
#ifdef AXR_USE_PLATFORM_WIN32
    closeWin32Window();
#else
    axrLogErrorLocation("Unknown platform.");
#endif

    if (AXR_FAILED(invokeConfigureWindowGraphicsCallback(false))) {
        axrLogErrorLocation("Failed to clean up window graphics.");
    }
}

AxrResult AxrWindowSystem::setup() {
#ifdef AXR_USE_PLATFORM_WIN32
    return setupWin32Window();
#else
    axrLogErrorLocation("Unknown platform.");
    return AXR_ERROR;
#endif
}

void AxrWindowSystem::processEvents() {
#ifdef AXR_USE_PLATFORM_WIN32
    processWin32Events();
#else
    axrLogErrorLocation("Unknown platform.");
#endif
}

void AxrWindowSystem::setConfigureWindowGraphicsCallback(
    void* userData,
    const ConfigureWindowGraphicsCallback_T function
) {
    if (m_ConfigureWindowGraphicsCallback != nullptr) {
        axrLogErrorLocation("ConfigureWindowGraphics callback has already been set.");
        return;
    }

    m_ConfigureWindowGraphicsCallback = function;
    m_ConfigureWindowGraphicsCallbackUserData = userData;
}

void AxrWindowSystem::resetConfigureWindowGraphicsCallback() {
    m_ConfigureWindowGraphicsCallback = nullptr;
    m_ConfigureWindowGraphicsCallbackUserData = nullptr;
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrWin32WindowSystem* AxrWindowSystem::getWin32WindowSystem() const {
    return m_Win32WindowSystem.get();
}
#endif

AxrResult AxrWindowSystem::getClientSize(uint32_t& width, uint32_t& height) const {
#ifdef AXR_USE_PLATFORM_WIN32
    return getWin32ClientSize(width, height);
#else
    axrLogErrorLocation("Unknown platform.");
    return AXR_ERROR;
#endif
}

// ---- Private Functions ----

AxrResult AxrWindowSystem::invokeConfigureWindowGraphicsCallback(const bool isWindowOpen) const {
    return m_ConfigureWindowGraphicsCallback(m_ConfigureWindowGraphicsCallbackUserData, isWindowOpen);
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrResult AxrWindowSystem::setupWin32Window() {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->setup();
}

bool AxrWindowSystem::isWin32WindowOpen() const {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return false;
    }

    return m_Win32WindowSystem->isWindowOpen();
}

AxrResult AxrWindowSystem::openWin32Window() {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->openWindow();
}

void AxrWindowSystem::closeWin32Window() {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    m_Win32WindowSystem->closeWindow();
}

void AxrWindowSystem::processWin32Events() {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    m_Win32WindowSystem->processEvents();
}

AxrResult AxrWindowSystem::getWin32ClientSize(uint32_t& width, uint32_t& height) const {
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->getClientSize(width, height);
}
#endif
