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
    if (config == nullptr) {
        axrLogErrorLocation("`config` is null.");
        return nullptr;
    }

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

AxrGraphicsSystem_T axrApplicationGetGraphicsSystem(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getGraphicsSystem();
}

AxrAssetCollection_T axrApplicationGetGlobalAssetCollection(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getGlobalAssetCollection();
}

AxrResult axrApplicationCreateScene(const AxrApplication_T app, const char* sceneName) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return AXR_ERROR;
    }

    return app->createScene(sceneName);
}

AxrScene_T axrApplicationFindScene(const AxrApplication_T app, const char* sceneName) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->findScene(sceneName);
}

AxrResult axrApplicationLoadScene(const AxrApplication_T app, const char* sceneName) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return AXR_ERROR;
    }

    return app->loadScene(sceneName);
}

AxrResult axrApplicationSetActiveScene(const AxrApplication_T app, const char* sceneName) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return AXR_ERROR;
    }

    return app->setActiveScene(sceneName);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrApplication::AxrApplication(const AxrApplicationConfig& config) :
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(config.ApplicationVersion),
    m_WindowSystem(
        AxrWindowSystem::Config{
            .ApplicationName = config.ApplicationName,
            .WindowConfig = config.WindowSystemConfig
        }
    ),
    m_GraphicsSystem(
        AxrGraphicsSystem::Config{
            .ApplicationName = config.ApplicationName,
            .ApplicationVersion = config.ApplicationVersion,
            .WindowSystem = &m_WindowSystem,
            .GlobalAssetCollection = &m_GlobalAssetCollection,
            .GraphicsConfig = config.GraphicsSystemConfig,
        }
    ) {
}

AxrApplication::~AxrApplication() {
    m_GraphicsSystem.cleanup();
    m_WindowSystem.cleanup();
    m_GlobalAssetCollection.cleanup();
}

// ---- Public Functions ----

AxrResult AxrApplication::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_WindowSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = m_GraphicsSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
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

AxrAssetCollection_T AxrApplication::getGlobalAssetCollection() {
    return &m_GlobalAssetCollection;
}

AxrResult AxrApplication::createScene(const char* sceneName) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Scenes.contains(sceneName)) {
        axrLogErrorLocation("Scene already exists with the name: {0}.", sceneName);
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Scenes.insert(std::pair(sceneName, AxrScene(sceneName)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrScene_T AxrApplication::findScene(const char* sceneName) {
    const auto foundScene = m_Scenes.find(sceneName);
    if (foundScene == m_Scenes.end()) {
        axrLogErrorLocation("Failed to find scene with the name: {0}.", sceneName);
        return nullptr;
    }

    return &foundScene->second;
}

AxrResult AxrApplication::loadScene(const char* sceneName) {
    AxrScene_T foundScene = findScene(sceneName);
    if (foundScene == nullptr) {
        axrLogErrorLocation("Failed to find scene with the name: {0}.", sceneName);
        return AXR_ERROR;
    }

    return m_GraphicsSystem.loadScene(foundScene);
}

AxrResult AxrApplication::setActiveScene(const char* sceneName) {
    return m_GraphicsSystem.setActiveScene(sceneName);
}
