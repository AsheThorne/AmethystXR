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
    m_Platform(config.WindowConfig.Platform) {
#ifdef AXR_PLATFORM_WIN32
    if (config.WindowConfig.Platform == AXR_WINDOW_PLATFORM_WIN32) {
        m_Win32WindowSystem = std::make_unique<AxrWin32WindowSystem>(
            AxrWin32WindowSystem::Config{
                .ApplicationName = config.ApplicationName,
                .Width = config.WindowConfig.Width,
                .Height = config.WindowConfig.Height,
            }
        );
    }
#endif
}

// ---- Public Functions ----

AxrResult AxrWindowSystem::setup() {
    switch (m_Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            return setupWin32Window();
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return AXR_ERROR;
        }
    }
}

bool AxrWindowSystem::isWindowOpen() const {
    switch (m_Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            return isWin32WindowOpen();
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return false;
        }
    }
}

AxrResult AxrWindowSystem::openWindow() {
    AxrResult axrResult = AXR_ERROR;
    
    switch (m_Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            axrResult = openWin32Window();
            break;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            axrResult = AXR_ERROR;
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    invokeOnWindowOpenedCallback();

    return axrResult;
}

void AxrWindowSystem::closeWindow() {
    switch (m_Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            closeWin32Window();
            return;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return;
        }
    }
}

void AxrWindowSystem::processEvents() {
    switch (m_Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            processWin32Events();
            return;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform.");
            return;
        }
    }
}

void AxrWindowSystem::addOnWindowOpenedCallback(
    void* userData,
    const OnWindowOpenedEvent_T::CallbackFunction_T& function
) {
    m_OnWindowOpenedEvent.addCallback(userData, function);
}

void AxrWindowSystem::removeOnWindowOpenedCallback(const OnWindowOpenedEvent_T::CallbackFunction_T& function) {
    m_OnWindowOpenedEvent.removeCallback(function);
}

// ---- Private Functions ----

void AxrWindowSystem::invokeOnWindowOpenedCallback() {
    m_OnWindowOpenedEvent.invoke(this);
}

AxrResult AxrWindowSystem::setupWin32Window() {
#ifdef AXR_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->setup();
#elif
 axrLogErrorLocation(
        "Windows platform not supported."
    );
    return AXR_ERROR;
#endif
}

bool AxrWindowSystem::isWin32WindowOpen() const {
#ifdef AXR_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return false;
    }

    return m_Win32WindowSystem->isWindowOpen();
#elif
    axrLogErrorLocation(
           "Windows platform not supported."
       );
    return false;
#endif
}

AxrResult AxrWindowSystem::openWin32Window() {
#ifdef AXR_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return AXR_ERROR;
    }

    return m_Win32WindowSystem->openWindow();
#elif
    axrLogErrorLocation(
           "Windows platform not supported."
       );
    return AXR_ERROR;
#endif
}

void AxrWindowSystem::closeWin32Window() {
#ifdef AXR_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    m_Win32WindowSystem->closeWindow();
#elif
    axrLogErrorLocation(
           "Windows platform not supported."
       );
    return;
#endif
}

void AxrWindowSystem::processWin32Events() {
#ifdef AXR_PLATFORM_WIN32
    if (m_Win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32WindowSystem is null.");
        return;
    }

    m_Win32WindowSystem->processEvents();
#elif
    axrLogErrorLocation(
           "Windows platform not supported."
       );
    return;
#endif
}
