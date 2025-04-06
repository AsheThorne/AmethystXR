#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanLoadedScenesCollection.hpp"
#include "axr/logger.h"

// ---- Special Functions ----

AxrVulkanLoadedScenesCollection::AxrVulkanLoadedScenesCollection():
    m_Device(VK_NULL_HANDLE),
    m_Dispatch(nullptr),
    m_IsWindowReady(false) {
}

AxrVulkanLoadedScenesCollection::~AxrVulkanLoadedScenesCollection() {
    resetSetup();
}

// ---- Public Functions ----

const AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::findLoadedScene(const char* sceneName) {
    const auto foundScene = findLoadedSceneIterator(sceneName);
    if (foundScene == m_LoadedScenes.end()) {
        return nullptr;
    }

    return *foundScene;
}

AxrResult AxrVulkanLoadedScenesCollection::setup(const SetupConfig& config) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogErrorLocation("Device isn't null.");
        return AXR_ERROR;
    }

    if (config.Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.Device is null.");
        return AXR_ERROR;
    }

    if (m_Dispatch != VK_NULL_HANDLE) {
        axrLogErrorLocation("Dispatch isn't null.");
        return AXR_ERROR;
    }

    if (config.Dispatch == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.Dispatch is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    m_Device = config.Device;
    m_Dispatch = config.Dispatch;

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::resetSetup() {
    clear();
    resetSetupWindowData();

    m_Device = VK_NULL_HANDLE;
    m_Dispatch = nullptr;
}

AxrResult AxrVulkanLoadedScenesCollection::loadScene(
    const char* sceneName,
    const AxrAssetCollection_T assetCollection,
    AxrVulkanSceneData* sharedSceneData
) {
    AxrResult axrResult = AXR_SUCCESS;

    AxrVulkanSceneData* sceneData = createSceneData(
        sceneName,
        assetCollection,
        sharedSceneData
    );
    m_LoadedScenes.push_back(sceneData);

    axrResult = sceneData->loadScene();
    if (AXR_FAILED(axrResult)) {
        unloadScene(sceneData->getSceneName());
        return axrResult;
    }

    if (m_IsWindowReady) {
        axrResult = sceneData->loadWindowData();
        if (AXR_FAILED(axrResult)) {
            unloadScene(sceneData->getSceneName());
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::unloadScene(const char* sceneName) {
    const auto foundScene = findLoadedSceneIterator(sceneName);
    if (foundScene == m_LoadedScenes.end()) {
        axrLogErrorLocation("Scene named: {0} not found.", sceneName);
        return;
    }

    (*foundScene)->unloadWindowData();
    (*foundScene)->unloadScene();
    destroySceneData(*foundScene);

    m_LoadedScenes.erase(foundScene);
}

void AxrVulkanLoadedScenesCollection::clear() {
    for (AxrVulkanSceneData*& scene : m_LoadedScenes) {
        scene->unloadWindowData();
        scene->unloadScene();
        destroySceneData(scene);
    }

    m_LoadedScenes.clear();
}

AxrResult AxrVulkanLoadedScenesCollection::setupWindowData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_IsWindowReady) {
        axrLogErrorLocation("Window data is already set up.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // TODO: set the required window scene data here
    m_IsWindowReady = true;

    return loadAllWindowSceneData();
}

void AxrVulkanLoadedScenesCollection::resetSetupWindowData() {
    m_IsWindowReady = false;

    unloadAllWindowSceneData();
}

// ---- Private Functions ----

std::vector<AxrVulkanSceneData*>::iterator AxrVulkanLoadedScenesCollection::findLoadedSceneIterator(
    const char* sceneName
) {
    for (auto i = m_LoadedScenes.begin(); i != m_LoadedScenes.end(); ++i) {
        if (strcmp((*i)->getSceneName(), sceneName) == 0) {
            return i;
        }
    }

    return m_LoadedScenes.end();
}

AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::createSceneData(
    const char* sceneName,
    const AxrAssetCollection_T assetCollection,
    AxrVulkanSceneData* sharedSceneData
) const {
    return new AxrVulkanSceneData(
        {
            .SceneName = sceneName,
            .AssetCollection = assetCollection,
            .SharedVulkanSceneData = sharedSceneData,
            .Device = m_Device,
            .DispatchHandle = m_Dispatch
        }
    );
}

void AxrVulkanLoadedScenesCollection::destroySceneData(AxrVulkanSceneData*& sceneData) const {
    if (sceneData == nullptr) return;

    delete sceneData;
    sceneData = nullptr;
}

AxrResult AxrVulkanLoadedScenesCollection::loadAllWindowSceneData() const {
    AxrResult axrResult = AXR_SUCCESS;

    for (AxrVulkanSceneData* scene : m_LoadedScenes) {
        axrResult = scene->loadWindowData();
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to load all window scene data.");
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::unloadAllWindowSceneData() const {
    for (AxrVulkanSceneData* scene : m_LoadedScenes) {
        scene->unloadWindowData();
    }
}

#endif
