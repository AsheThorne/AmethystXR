#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanLoadedScenesCollection.hpp"
#include "axr/logger.h"

// ---- Special Functions ----

AxrVulkanLoadedScenesCollection::AxrVulkanLoadedScenesCollection():
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_MaxFramesInFlight(0),
    m_Dispatch(nullptr),
    m_IsSetup(false),
    m_WindowRenderPass(VK_NULL_HANDLE),
    m_ActiveScene(nullptr) {
}

AxrVulkanLoadedScenesCollection::~AxrVulkanLoadedScenesCollection() {
    resetSetup();
}

// ---- Public Functions ----

bool AxrVulkanLoadedScenesCollection::isSetup() const {
    return m_IsSetup;
}

AxrResult AxrVulkanLoadedScenesCollection::setup(const SetupConfig& config) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice != VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device isn't null.");
        return AXR_ERROR;
    }

    if (config.PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.PhysicalDevice is null.");
        return AXR_ERROR;
    }

    if (m_Device != VK_NULL_HANDLE) {
        axrLogErrorLocation("Device isn't null.");
        return AXR_ERROR;
    }

    if (config.Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.Device is null.");
        return AXR_ERROR;
    }

    if (m_TransferCommandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Transfer command pool isn't null.");
        return AXR_ERROR;
    }

    if (config.TransferCommandPool == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.TransferCommandPool is null.");
        return AXR_ERROR;
    }

    if (m_TransferQueue != VK_NULL_HANDLE) {
        axrLogErrorLocation("Transfer queue isn't null.");
        return AXR_ERROR;
    }

    if (config.TransferQueue == VK_NULL_HANDLE) {
        axrLogErrorLocation("config.TransferQueue is null.");
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

    m_PhysicalDevice = config.PhysicalDevice;
    m_Device = config.Device;
    m_TransferCommandPool = config.TransferCommandPool;
    m_TransferQueue = config.TransferQueue;
    m_MaxFramesInFlight = config.MaxFramesInFlight;
    m_Dispatch = config.Dispatch;

    m_IsSetup = true;

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::resetSetup() {
    clear();
    resetSetupWindowData();

    m_IsSetup = false;

    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_MaxFramesInFlight = 0;
    m_Dispatch = nullptr;
}

AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::getGlobalSceneData() const {
    if (m_LoadedScenes.empty()) {
        return nullptr;
    }

    // The global scene data should always be the first loaded scene
    return m_LoadedScenes[0];
}

AxrResult AxrVulkanLoadedScenesCollection::loadGlobalSceneData(const AxrAssetCollection_T assetCollection) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    AxrVulkanSceneData* globalSceneData = getGlobalSceneData();
    if (globalSceneData != nullptr) {
        axrLogErrorLocation("Global scene data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    AxrVulkanSceneData* sceneData = createSceneData(
        m_GlobalSceneName,
        assetCollection,
        nullptr,
        globalSceneData
    );
    m_LoadedScenes.push_back(sceneData);

    axrResult = sceneData->loadScene();
    if (AXR_FAILED(axrResult)) {
        unloadScene(sceneData->getSceneName());
        return axrResult;
    }

    if (isWindowReady()) {
        axrResult = sceneData->loadWindowData(m_WindowRenderPass);
        if (AXR_FAILED(axrResult)) {
            unloadScene(sceneData->getSceneName());
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanLoadedScenesCollection::loadScene(
    const std::string& sceneName,
    const AxrAssetCollection_T assetCollection,
    entt::registry* ecsRegistryHandle
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    AxrVulkanSceneData* globalSceneData = getGlobalSceneData();
    if (globalSceneData == nullptr) {
        axrLogErrorLocation("Global scene data is null. Load global scene data first.");
        return AXR_ERROR;
    }

    for (const AxrVulkanSceneData* loadedScene : m_LoadedScenes) {
        if (loadedScene->getSceneName() == sceneName) {
            axrLogErrorLocation("Scene named \"{0}\" has already been loaded", sceneName.c_str());
            return AXR_ERROR;
        }
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    AxrVulkanSceneData* sceneData = createSceneData(
        sceneName,
        assetCollection,
        ecsRegistryHandle,
        globalSceneData
    );
    m_LoadedScenes.push_back(sceneData);

    axrResult = sceneData->loadScene();
    if (AXR_FAILED(axrResult)) {
        unloadScene(sceneData->getSceneName());
        return axrResult;
    }

    if (isWindowReady()) {
        axrResult = sceneData->loadWindowData(m_WindowRenderPass);
        if (AXR_FAILED(axrResult)) {
            unloadScene(sceneData->getSceneName());
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::unloadScene(const std::string& sceneName) {
    if (m_ActiveScene != nullptr && m_ActiveScene->getSceneName() == sceneName) {
        m_ActiveScene = nullptr;
    }

    const auto foundScene = findLoadedSceneIterator(sceneName);
    if (foundScene == m_LoadedScenes.end()) {
        axrLogErrorLocation("Scene named: {0} not found.", sceneName.c_str());
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
    m_ActiveScene = nullptr;
}

AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::findLoadedScene(const std::string& sceneName) {
    const auto foundScene = findLoadedSceneIterator(sceneName);
    if (foundScene == m_LoadedScenes.end()) {
        return nullptr;
    }

    return *foundScene;
}

AxrResult AxrVulkanLoadedScenesCollection::setActiveScene(const std::string& sceneName) {
    AxrVulkanSceneData* foundScene = findLoadedScene(sceneName);
    if (foundScene == nullptr) {
        axrLogErrorLocation("Failed to find loaded scene named: {0}.", sceneName.c_str());
        return AXR_ERROR;
    }

    AxrVulkanSceneData* globalSceneData = getGlobalSceneData();
    if (globalSceneData == nullptr) {
        axrLogErrorLocation("No global scene data found.");
        return AXR_ERROR;
    }

    const AxrResult axrResult = globalSceneData->onSetActiveScene(foundScene);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    m_ActiveScene = foundScene;

    return AXR_SUCCESS;
}

AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::getActiveScene() const {
    return m_ActiveScene;
}

AxrResult AxrVulkanLoadedScenesCollection::setupWindowData(const vk::RenderPass renderPass) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (isWindowReady()) {
        axrLogErrorLocation("Window data is already set up.");
        return AXR_ERROR;
    }

    if (m_WindowRenderPass != VK_NULL_HANDLE) {
        axrLogErrorLocation("Window render pass already exists.");
        return AXR_ERROR;
    }

    if (renderPass == VK_NULL_HANDLE) {
        axrLogErrorLocation("renderPass is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    m_WindowRenderPass = renderPass;
    // If anything new gets added here, make sure it also gets added to the isWindowReady() function

    return loadAllWindowSceneData();
}

void AxrVulkanLoadedScenesCollection::resetSetupWindowData() {
    m_WindowRenderPass = VK_NULL_HANDLE;

    unloadAllWindowSceneData();
}

// ---- Private Functions ----

std::vector<AxrVulkanSceneData*>::iterator AxrVulkanLoadedScenesCollection::findLoadedSceneIterator(
    const std::string& sceneName
) {
    for (auto i = m_LoadedScenes.begin(); i != m_LoadedScenes.end(); ++i) {
        if ((*i)->getSceneName() == sceneName) {
            return i;
        }
    }

    return m_LoadedScenes.end();
}

AxrVulkanSceneData* AxrVulkanLoadedScenesCollection::createSceneData(
    const std::string& sceneName,
    const AxrAssetCollection_T assetCollection,
    entt::registry* ecsRegistryHandle,
    AxrVulkanSceneData* globalSceneData
) const {
    return new AxrVulkanSceneData(
        {
            .SceneName = sceneName.c_str(),
            .AssetCollection = assetCollection,
            .EcsRegistryHandle = ecsRegistryHandle,
            .GlobalSceneData = globalSceneData,
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .TransferCommandPool = m_TransferCommandPool,
            .TransferQueue = m_TransferQueue,
            .MaxFramesInFlight = m_MaxFramesInFlight,
            .DispatchHandle = m_Dispatch
        }
    );
}

void AxrVulkanLoadedScenesCollection::destroySceneData(AxrVulkanSceneData*& sceneData) const {
    if (sceneData == nullptr) return;

    delete sceneData;
    sceneData = nullptr;
}

bool AxrVulkanLoadedScenesCollection::isWindowReady() const {
    return m_WindowRenderPass != VK_NULL_HANDLE;
}

AxrResult AxrVulkanLoadedScenesCollection::loadAllWindowSceneData() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!isWindowReady()) {
        axrLogErrorLocation("Window data is not ready.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (AxrVulkanSceneData* scene : m_LoadedScenes) {
        axrResult = scene->loadWindowData(m_WindowRenderPass);
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
