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
    m_Dispatch(nullptr),
    m_WindowRenderPass(VK_NULL_HANDLE) {
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
    m_Dispatch = config.Dispatch;

    return AXR_SUCCESS;
}

void AxrVulkanLoadedScenesCollection::resetSetup() {
    clear();
    resetSetupWindowData();

    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
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

    if (isWindowReady()) {
        axrResult = sceneData->loadWindowData(m_WindowRenderPass);
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

AxrResult AxrVulkanLoadedScenesCollection::setupWindowData(vk::RenderPass renderPass) {
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
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .TransferCommandPool = m_TransferCommandPool,
            .TransferQueue = m_TransferQueue,
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
