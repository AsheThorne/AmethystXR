// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "application.hpp"
#include "axr/logger.h"

#include <iostream>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrApplication_T axrCreateApplication(const AxrApplicationConfig* config) {
    return new AxrApplication(*config);
}

void axrDestroyApplication(AxrApplication_T* app) {
    if (app == nullptr || *app == nullptr) return;

    delete *app;
    *app = nullptr;
}

AxrResult axrApplicationSetup(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return AXR_ERROR;
    }

    return app->setup();
}

bool axrApplicationIsRunning(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return false;
    }

    return app->isRunning();
}

void axrApplicationProcessEvents(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return;
    }

    app->processEvents();
}

AxrWindowSystem_T axrApplicationGetWindowSystem(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getWindowSystem();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrApplication::AxrApplication(const AxrApplicationConfig& config) :
    m_Config(config),
    m_WindowSystem(
        {
            .ApplicationName = config.ApplicationName,
            .Platform = config.WindowSystemConfig.Platform,
            .Width = config.WindowSystemConfig.Width,
            .Height = config.WindowSystemConfig.Height,
        }
    ),
    m_GraphicsSystem(
        {
            .GraphicsApi = config.GraphicsSystemConfig.GraphicsApi,
            .ApplicationName = config.ApplicationName,
            .ApplicationVersion = config.ApplicationVersion,
        }
    ) {
}

// ---- Public Functions ----

AxrResult AxrApplication::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_WindowSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;
    
    axrResult = m_GraphicsSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    return axrResult;
}

bool AxrApplication::isRunning() const {
    // TODO: Check if the OpenXR session is running too
    return m_WindowSystem.isWindowOpen();
}

void AxrApplication::processEvents() {
    // TODO: Process OpenXR events too
    m_WindowSystem.processEvents();
}

AxrWindowSystem_T AxrApplication::getWindowSystem() {
    return &m_WindowSystem;
}

AxrGraphicsSystem_T AxrApplication::getGraphicsSystem() {
    return &m_GraphicsSystem;
}
