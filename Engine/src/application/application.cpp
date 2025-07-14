// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "application.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <chrono>

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

bool axrApplicationIsRunning(const AxrApplicationConst_T app) {
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

AxrXrSystem_T axrApplicationGetXrSystem(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getXrSystem();
}

AxrGraphicsSystem_T axrApplicationGetGraphicsSystem(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getGraphicsSystem();
}

AxrIOActionSystem_T axrApplicationGetIOActionSystem(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getIOActionSystem();
}

AxrAssetCollection_T axrApplicationGetGlobalAssetCollection(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return nullptr;
    }

    return app->getGlobalAssetCollection();
}

float axrApplicationGetDeltaTime(const AxrApplication_T app) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return 0;
    }

    return app->getDeltaTime();
}

AxrResult axrApplicationCreateScene(const AxrApplication_T app, char sceneName[AXR_MAX_SCENE_NAME_SIZE]) {
    if (app == nullptr) {
        axrLogErrorLocation("`app` is null.");
        return AXR_ERROR;
    }

    return app->createScene(sceneName == nullptr ? "" : sceneName);
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
    m_GraphicsSystem(
        AxrGraphicsSystem::Config{
            .ApplicationName = config.ApplicationName == nullptr ? "" : config.ApplicationName,
            .ApplicationVersion = config.ApplicationVersion,
            .WindowSystem = config.WindowSystemConfig == nullptr ? nullptr : &m_WindowSystem,
            .XrSystem = config.XrSystemConfig == nullptr ? nullptr : &m_XrSystem,
            .GlobalAssetCollection = &m_GlobalAssetCollection,
            .GraphicsConfig = config.GraphicsSystemConfig,
        }
    ),
    m_WindowSystem(
        [&]() -> AxrWindowSystem {
            if (config.WindowSystemConfig == nullptr) {
                return AxrWindowSystem(nullptr);
            }

            return AxrWindowSystem(
                AxrWindowSystem::Config{
                    .ApplicationName = config.ApplicationName,
                    .IOActionSystem = &m_IOActionSystem,
                    .Width = config.WindowSystemConfig->Width,
                    .Height = config.WindowSystemConfig->Height,
                }
            );
        }()
    ),
    m_XrSystem(
        [config]() -> AxrXrSystem {
            if (config.XrSystemConfig == nullptr) {
                return AxrXrSystem(nullptr);
            }

            return AxrXrSystem(
                AxrXrSystem::Config{
                    .ApplicationName = config.ApplicationName,
                    .ApplicationVersion = config.ApplicationVersion,
                    .GraphicsApi = config.GraphicsSystemConfig.GraphicsApi,
                    .StageReferenceSpace = config.XrSystemConfig->StageReferenceSpace,
                    .ApiLayerCount = config.XrSystemConfig->ApiLayerCount,
                    .ApiLayers = config.XrSystemConfig->ApiLayers,
                    .ExtensionCount = config.XrSystemConfig->ExtensionCount,
                    .Extensions = config.XrSystemConfig->Extensions
                }
            );
        }()
    ),
    m_IOActionSystem(
        AxrIOActionSystem::Config{
            .XrSystem = config.XrSystemConfig == nullptr ? nullptr : &m_XrSystem,
            .ActionSetCount = config.IOActionSystemConfig.IOActionSetCount,
            .ActionSets = config.IOActionSystemConfig.IOActionSets,
            .XrInteractionProfileCount = config.IOActionSystemConfig.XrInteractionProfileCount,
            .XrInteractionProfiles = config.IOActionSystemConfig.XrInteractionProfiles
        }
    ),
    m_DeltaTime(0) {
}

AxrApplication::~AxrApplication() {
    m_GraphicsSystem.resetSetup();
    m_IOActionSystem.resetSetup();
    m_WindowSystem.resetSetup();
    m_XrSystem.resetSetup();
    m_GlobalAssetCollection.cleanup();
}

// ---- Public Functions ----

AxrResult AxrApplication::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = setupGlobalAssetCollection();
    if (AXR_FAILED(axrResult)) return axrResult;

    if (m_WindowSystem.isValid()) {
        axrResult = m_WindowSystem.setup();
        if (AXR_FAILED(axrResult)) return axrResult;
    }

    if (m_XrSystem.isValid()) {
        axrResult = m_XrSystem.setup();
        if (AXR_FAILED(axrResult)) return axrResult;
    }

    axrResult = m_IOActionSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = m_GraphicsSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}

bool AxrApplication::isRunning() const {
    bool windowIsOpen = false;
    bool xrSessionIsRunning = false;

    if (m_WindowSystem.isValid()) {
        windowIsOpen = m_WindowSystem.isWindowOpen();
    }

    if (m_XrSystem.isValid()) {
        xrSessionIsRunning = m_XrSystem.isXrSessionRunning();
    }

    return windowIsOpen || xrSessionIsRunning;
}

void AxrApplication::processEvents() {
    m_IOActionSystem.newFrameStarted();

    if (m_WindowSystem.isValid()) {
        m_WindowSystem.processEvents();
    }
    if (m_XrSystem.isValid()) {
        m_XrSystem.processEvents();
    }

    m_IOActionSystem.processEvents();

    static std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
    const std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
    m_DeltaTime = deltaTime.count();
    lastFrameTime = currentFrameTime;
}

AxrWindowSystem_T AxrApplication::getWindowSystem() {
    return &m_WindowSystem;
}

AxrXrSystem_T AxrApplication::getXrSystem() {
    return &m_XrSystem;
}

AxrGraphicsSystem_T AxrApplication::getGraphicsSystem() {
    return &m_GraphicsSystem;
}

AxrIOActionSystem_T AxrApplication::getIOActionSystem() {
    return &m_IOActionSystem;
}

AxrAssetCollection_T AxrApplication::getGlobalAssetCollection() {
    return &m_GlobalAssetCollection;
}

float AxrApplication::getDeltaTime() const {
    return m_DeltaTime;
}

AxrResult AxrApplication::createScene(const std::string& sceneName) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Scenes.contains(sceneName)) {
        axrLogErrorLocation("Scene already exists with the name: {0}.", sceneName.c_str());
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

AxrScene_T AxrApplication::findScene(const std::string& sceneName) {
    const auto foundScene = m_Scenes.find(sceneName);
    if (foundScene == m_Scenes.end()) {
        axrLogErrorLocation("Failed to find scene with the name: {0}.", sceneName.c_str());
        return nullptr;
    }

    return &foundScene->second;
}

AxrResult AxrApplication::loadScene(const std::string& sceneName) {
    const AxrScene_T foundScene = findScene(sceneName);
    if (foundScene == nullptr) {
        axrLogErrorLocation("Failed to find scene with the name: {0}.", sceneName.c_str());
        return AXR_ERROR;
    }

    return m_GraphicsSystem.loadScene(foundScene);
}

AxrResult AxrApplication::setActiveScene(const std::string& sceneName) {
    return m_GraphicsSystem.setActiveScene(sceneName);
}

// ---- Private Functions ----

AxrResult AxrApplication::setupGlobalAssetCollection() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_GlobalAssetCollection.createImageSampler(AXR_ENGINE_ASSET_IMAGE_SAMPLER_NEAREST_REPEAT);
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = m_GlobalAssetCollection.createImageSampler(AXR_ENGINE_ASSET_IMAGE_SAMPLER_LINEAR_REPEAT);
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = m_GlobalAssetCollection.createImage(AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE);
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}
