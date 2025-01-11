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
        axrLogError("axrWindowSystemIsWindowOpen - `windowSystem` is null.");
        return false;
    }

    return windowSystem->isWindowOpen();
}

AxrResult axrWindowSystemOpenWindow(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogError("axrWindowSystemOpenWindow - `windowSystem` is null.");
        return AXR_ERROR;
    }

    return windowSystem->openWindow();
}

void axrWindowSystemCloseWindow(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogError("axrWindowSystemCloseWindow - `windowSystem` is null.");
        return;
    }

    windowSystem->closeWindow();
}

void axrWindowSystemProcessEvents(const AxrWindowSystem_T windowSystem) {
    if (windowSystem == nullptr) {
        axrLogError("axrWindowSystemProcessEvents - `windowSystem` is null.");
        return;
    }

    windowSystem->processEvents();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrWindowSystem::AxrWindowSystem(const Config& config):
#ifdef AXR_PLATFORM_WINDOWS
    m_Win32WindowSystem(
        {
            .ApplicationName = config.ApplicationName,
            .Width = config.Width,
            .Height = config.Height,
        }
    ),
#endif
    m_Config(config) {
}

// ---- Public Functions ----

AxrResult AxrWindowSystem::setup() {
    switch (m_Config.Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            return setupWin32Window();
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogError("AxrWindowSystem::setup - Unknown platform.");
            return AXR_ERROR;
        }
    }
}

bool AxrWindowSystem::isWindowOpen() const {
    switch (m_Config.Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            return isWin32WindowOpen();
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogError("AxrWindowSystem::isWindowOpen - Unknown platform.");
            return false;
        }
    }
}

AxrResult AxrWindowSystem::openWindow() {
    switch (m_Config.Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            return openWin32Window();
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogError("AxrWindowSystem::openWindow - Unknown platform.");
            return AXR_ERROR;
        }
    }
}

void AxrWindowSystem::closeWindow() {
    switch (m_Config.Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            closeWin32Window();
            return;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogError("AxrWindowSystem::closeWindow - Unknown platform.");
            return;
        }
    }
}

void AxrWindowSystem::processEvents() {
    switch (m_Config.Platform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            processWin32Events();
            return;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogError("AxrWindowSystem::processEvents - Unknown platform.");
            return;
        }
    }
}

// ---- Private Functions ----

AxrResult AxrWindowSystem::setupWin32Window() {
#ifdef AXR_PLATFORM_WINDOWS
    return m_Win32WindowSystem.setup();
#elif
 axrLogError(
        "AxrWindowSystem::setupWin32Window - Windows platform not supported."
    );
    return AXR_ERROR;
#endif
}

bool AxrWindowSystem::isWin32WindowOpen() const {
#ifdef AXR_PLATFORM_WINDOWS
    return m_Win32WindowSystem.isWindowOpen();
#elif
    axrLogError(
           "AxrWindowSystem::isWin32WindowOpen - Windows platform not supported."
       );
    return AXR_ERROR;
#endif
}

AxrResult AxrWindowSystem::openWin32Window() {
#ifdef AXR_PLATFORM_WINDOWS
    return m_Win32WindowSystem.openWindow();
#elif
    axrLogError(
           "AxrWindowSystem::openWin32Window - Windows platform not supported."
       );
    return AXR_ERROR;
#endif
}

void AxrWindowSystem::closeWin32Window() {
#ifdef AXR_PLATFORM_WINDOWS
    m_Win32WindowSystem.closeWindow();
#elif
    axrLogError(
           "AxrWindowSystem::closeWin32Window - Windows platform not supported."
       );
    return AXR_ERROR;
#endif
}

void AxrWindowSystem::processWin32Events() {
#ifdef AXR_PLATFORM_WINDOWS
    m_Win32WindowSystem.processEvents();
#elif
    axrLogError(
           "AxrWindowSystem::processWin32Events - Windows platform not supported."
       );
    return AXR_ERROR;
#endif
}
