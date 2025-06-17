#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSceneData.hpp"
#include "axr/logger.h"
#include "../../../assets/assetCollection.hpp"
#include "../../../assets/material.hpp"
#include "../vulkanUtils.hpp"
#include "../../../utils.hpp"

// ---- Special Functions ----

AxrVulkanSceneData::AxrVulkanSceneData(const Config& config):
    m_SceneName(config.SceneName),
    m_AssetCollection(config.AssetCollection),
    m_EcsRegistryHandle(config.EcsRegistryHandle),
    m_GlobalSceneData(config.GlobalSceneData),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_GraphicsCommandPool(config.GraphicsCommandPool),
    m_GraphicsQueue(config.GraphicsQueue),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_MaxFramesInFlight(config.MaxFramesInFlight),
    m_MaxSamplerAnisotropy(config.MaxSamplerAnisotropy),
    m_DispatchHandle(config.DispatchHandle),
    m_IsWindowDataLoaded(false) {
}

AxrVulkanSceneData::~AxrVulkanSceneData() {
    unloadScene();
}

// ---- Public Functions ----

const std::string& AxrVulkanSceneData::getSceneName() const {
    return m_SceneName;
}

AxrResult AxrVulkanSceneData::loadScene() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_AssetCollection->loadAssets(AXR_GRAPHICS_API_VULKAN);
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    // TODO: Maybe validate push constant buffers?

    axrResult = createAllUniformBufferData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    axrResult = createAllModelData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    axrResult = createAllImageSamplerData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    axrResult = createAllImageData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    axrResult = createAllMaterialLayoutData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    axrResult = createAllMaterialData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    // TODO: Aren't we missing the writeAllDescriptorSets??

    axrResult = createAllMaterialsForRendering();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadScene() {
    // TODO: See if we can wait for all the scene specific fences to be finished instead of doing this.
    const vk::Result vkResult = m_Device.waitIdle(*m_DispatchHandle);
    axrLogVkResult(vkResult, "m_Device.waitIdle");

    // TODO: Unload OpenXR data
    unloadWindowData();

    destroyAllMaterialsForRendering();
    destroyAllMaterialData();
    destroyAllMaterialLayoutData();
    destroyAllImageData();
    destroyAllImageSamplerData();
    destroyAllModelData();
    destroyAllUniformBufferData();
    if (m_AssetCollection != nullptr) {
        m_AssetCollection->unloadAssets();
    }
}

AxrResult AxrVulkanSceneData::loadWindowData(const vk::RenderPass renderPass) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createAllWindowUniformBufferData();
    if (AXR_FAILED(axrResult)) {
        unloadWindowData();
        return axrResult;
    }

    axrResult = createAllWindowMaterialData(renderPass);
    if (AXR_FAILED(axrResult)) {
        unloadWindowData();
        return axrResult;
    }

    axrResult = writeAllWindowDescriptorSets();
    if (AXR_FAILED(axrResult)) {
        unloadWindowData();
        return axrResult;
    }

    m_IsWindowDataLoaded = true;

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadWindowData() {
    m_IsWindowDataLoaded = false;

    // TODO: See if we can wait for all the scene specific fences to be finished instead of doing this.
    const vk::Result vkResult = m_Device.waitIdle(*m_DispatchHandle);
    axrLogVkResult(vkResult, "m_Device.waitIdle");

    resetAllWindowDescriptorSets();
    destroyAllWindowMaterialData();
    destroyAllWindowUniformBufferData();
}

const std::unordered_map<std::string, AxrVulkanSceneData::MaterialForRendering>&
AxrVulkanSceneData::getMaterialsForRendering() const {
    return m_MaterialsForRendering;
}

AxrResult AxrVulkanSceneData::setWindowUniformBufferData(
    const std::string& bufferName,
    const uint32_t frameIndex,
    const vk::DeviceSize offset,
    const vk::DeviceSize dataSize,
    const void* data
) const {
    const AxrVulkanUniformBufferData* foundUniformBufferData = findWindowUniformBufferData_shared(bufferName);
    if (foundUniformBufferData == nullptr) {
        axrLogErrorLocation("Window uniform buffer does not exist.");
        return AXR_ERROR;
    }

    const AxrResult axrResult = foundUniformBufferData->setData(frameIndex, offset, dataSize, data);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation(
            "Failed to set window uniform buffer data for buffer named: {0}. At index: {1}",
            bufferName.c_str(),
            frameIndex
        );
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::onSetActiveScene(const AxrVulkanSceneData* activeSceneHandle) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = writeAllSceneSpecificDescriptorSets(AXR_PLATFORM_TYPE_WINDOW, activeSceneHandle);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    // TODO: Write all for XR too, if it's loaded

    return AXR_SUCCESS;
}

const AxrPushConstantBuffer* AxrVulkanSceneData::findPushConstantBuffer_shared(const std::string& name) const {
    if (name.empty()) return nullptr;

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return nullptr;
    }

    const AxrPushConstantBuffer* foundPushConstantBuffer = m_AssetCollection->findPushConstantBuffer(name);

    if (foundPushConstantBuffer != nullptr) {
        return foundPushConstantBuffer;
    }

    if (m_GlobalSceneData != nullptr) {
        foundPushConstantBuffer = m_GlobalSceneData->findPushConstantBuffer_shared(name);

        if (foundPushConstantBuffer != nullptr) {
            return foundPushConstantBuffer;
        }
    }

    return nullptr;
}

// ---- Private Functions ----

bool AxrVulkanSceneData::isThisGlobalSceneData() const {
    return m_GlobalSceneData == nullptr;
}

bool AxrVulkanSceneData::isPlatformLoaded(const AxrPlatformType platformType) const {
    switch (platformType) {
        case AXR_PLATFORM_TYPE_WINDOW: {
            return m_IsWindowDataLoaded;
        }
        case AXR_PLATFORM_TYPE_XR_DEVICE: {
            // NOTE: Add m_IsXrDataLoaded
            return false;
        }
        case AXR_PLATFORM_TYPE_UNDEFINED:
        default: {
            axrLogErrorLocation("Unknown platform type");
            return false;
        }
    }
}

void AxrVulkanSceneData::destroyUniformBufferData(
    std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferData
) {
    for (auto& [name, data] : uniformBufferData) {
        destroyUniformBufferData(data);
    }
    uniformBufferData.clear();
}

AxrResult AxrVulkanSceneData::createAllUniformBufferData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_UniformBufferData.empty()) {
        axrLogErrorLocation("Uniform buffer data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllUniformBufferData();
    if (AXR_FAILED(axrResult)) {
        destroyAllUniformBufferData();
        return axrResult;
    }

    for (auto& [name, data] : m_UniformBufferData) {
        axrResult = createUniformBufferData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllUniformBufferData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllUniformBufferData() {
    destroyUniformBufferData(m_UniformBufferData);
}

AxrResult AxrVulkanSceneData::initializeAllUniformBufferData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_UniformBufferData.empty()) {
        axrLogErrorLocation("Uniform buffer data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (const auto& [bufferName, buffer] : m_AssetCollection->getUniformBuffers()) {
        axrResult = initializeUniformBufferData(
            &buffer,
            AXR_ENGINE_ASSET_UNDEFINED,
            m_UniformBufferData
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize uniform buffer data.");
        destroyAllUniformBufferData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeUniformBufferData(
    const AxrUniformBuffer* uniformBufferHandle,
    const AxrEngineAssetEnum engineAsset,
    std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferDataCollection
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (
        (uniformBufferHandle == nullptr && engineAsset == AXR_ENGINE_ASSET_UNDEFINED) ||
        (uniformBufferHandle != nullptr && engineAsset != AXR_ENGINE_ASSET_UNDEFINED)
    ) {
        axrLogErrorLocation("Either a uniformBufferHandle must be defined, or a uniformBufferSize must be defined.");
        return AXR_ERROR;
    }

    if (engineAsset != AXR_ENGINE_ASSET_UNDEFINED && !axrEngineAssetIsUniformBuffer(engineAsset)) {
        axrLogErrorLocation("Engine asset is not a uniform buffer.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const AxrVulkanUniformBufferData::Config uniformBufferDataConfig{
        .UniformBufferHandle = uniformBufferHandle,
        .UniformBufferEngineAsset = engineAsset,
        .MaxFramesInFlight = m_MaxFramesInFlight,
        .PhysicalDevice = m_PhysicalDevice,
        .Device = m_Device,
        .TransferCommandPool = m_TransferCommandPool,
        .TransferQueue = m_TransferQueue,
        .DispatchHandle = m_DispatchHandle,
    };

    AxrVulkanUniformBufferData uniformBufferData(uniformBufferDataConfig);

    uniformBufferDataCollection.insert(
        std::pair(
            uniformBufferData.getName(),
            std::move(uniformBufferData)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createUniformBufferData(AxrVulkanUniformBufferData& uniformBufferData) {
    const AxrResult axrResult = uniformBufferData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyUniformBufferData(uniformBufferData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyUniformBufferData(AxrVulkanUniformBufferData& uniformBufferData) {
    uniformBufferData.destroyData();
}

const AxrVulkanUniformBufferData* AxrVulkanSceneData::findUniformBufferData_shared(
    const std::string& name,
    const AxrPlatformType platformType
) const {
    const auto foundUniformBufferDataIt = m_UniformBufferData.find(name);
    if (foundUniformBufferDataIt != m_UniformBufferData.end()) {
        return &foundUniformBufferDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        // platform type is undefined here because we don't want to search through the platform uniform buffers globally yet.
        const auto foundUniformBufferData = m_GlobalSceneData->findUniformBufferData_shared(
            name,
            AXR_PLATFORM_TYPE_UNDEFINED
        );

        if (foundUniformBufferData != nullptr) {
            return foundUniformBufferData;
        }
    }

    switch (platformType) {
        case AXR_PLATFORM_TYPE_WINDOW: {
            const auto foundUniformBufferData = findWindowUniformBufferData_shared(name);

            if (foundUniformBufferData != nullptr) {
                return foundUniformBufferData;
            }
            break;
        }
        case AXR_PLATFORM_TYPE_XR_DEVICE: {
            axrLogErrorLocation("Platform type 'Xr Device' Hasn't been implemented yet.");
            break;
        }
        case AXR_PLATFORM_TYPE_UNDEFINED: {
            // Nothing needs to be done here. It's valid to use an undefined type here.
            break;
        }
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown platform type.");
            break;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::createAllWindowUniformBufferData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_WindowUniformBufferData.empty()) {
        axrLogErrorLocation("Window uniform buffer data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllWindowUniformBufferData();
    if (AXR_FAILED(axrResult)) {
        destroyAllWindowUniformBufferData();
        return axrResult;
    }

    for (auto& [name, data] : m_WindowUniformBufferData) {
        axrResult = createUniformBufferData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllWindowUniformBufferData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllWindowUniformBufferData() {
    destroyUniformBufferData(m_WindowUniformBufferData);
}

AxrResult AxrVulkanSceneData::initializeAllWindowUniformBufferData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_WindowUniformBufferData.empty()) {
        axrLogErrorLocation("Window uniform buffer data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    if (!isThisGlobalSceneData()) {
        // ---- Scene Specific Uniform Buffers ----

        axrResult = initializeUniformBufferData(
            nullptr,
            AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA,
            m_WindowUniformBufferData
        );
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize uniform buffer data.");
        destroyAllWindowUniformBufferData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

const AxrVulkanUniformBufferData* AxrVulkanSceneData::findWindowUniformBufferData_shared(
    const std::string& name
) const {
    auto foundUniformBufferDataIt = m_WindowUniformBufferData.find(name);
    if (foundUniformBufferDataIt != m_WindowUniformBufferData.end()) {
        return &foundUniformBufferDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        const auto foundUniformBufferData = m_GlobalSceneData->findWindowUniformBufferData_shared(name);

        if (foundUniformBufferData != nullptr) {
            return foundUniformBufferData;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::createAllModelData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ModelData.empty()) {
        axrLogErrorLocation("Model data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllModelData();
    if (AXR_FAILED(axrResult)) {
        destroyAllModelData();
        return axrResult;
    }

    for (auto& [name, data] : m_ModelData) {
        axrResult = createModelData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllModelData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllModelData() {
    for (auto& [name, data] : m_ModelData) {
        destroyModelData(data);
    }
    m_ModelData.clear();
}

AxrResult AxrVulkanSceneData::initializeAllModelData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ModelData.empty()) {
        axrLogErrorLocation("Model data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (const auto& [modelName, model] : m_AssetCollection->getModels()) {
        axrResult = initializeModelData(model);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize model data.");
        destroyAllModelData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeModelData(const AxrModel& model) {
    const std::string modelName = model.getName();
    if (m_ModelData.contains(modelName)) return AXR_SUCCESS;

    const AxrVulkanModelData::Config modelDataConfig{
        .Name = modelName,
        .ModelHandle = &model,
        .PhysicalDevice = m_PhysicalDevice,
        .Device = m_Device,
        .TransferCommandPool = m_TransferCommandPool,
        .TransferQueue = m_TransferQueue,
        .DispatchHandle = m_DispatchHandle,
    };

    m_ModelData.insert(
        std::pair(
            modelName,
            AxrVulkanModelData(modelDataConfig)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createModelData(AxrVulkanModelData& modelData) {
    const AxrResult axrResult = modelData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyModelData(modelData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyModelData(AxrVulkanModelData& modelData) {
    modelData.destroyData();
}

const AxrVulkanModelData* AxrVulkanSceneData::findModelData_shared(const std::string& name) const {
    const auto foundModelDataIt = m_ModelData.find(name);
    if (foundModelDataIt != m_ModelData.end()) {
        return &foundModelDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        const auto foundModelData = m_GlobalSceneData->findModelData_shared(name);

        if (foundModelData != nullptr) {
            return foundModelData;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::createAllImageData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ImageData.empty()) {
        axrLogErrorLocation("Image data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllImageData();
    if (AXR_FAILED(axrResult)) {
        destroyAllImageData();
        return axrResult;
    }

    for (auto& [name, data] : m_ImageData) {
        axrResult = createImageData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllImageData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllImageData() {
    for (auto& [name, data] : m_ImageData) {
        destroyImageData(data);
    }
    m_ImageData.clear();
}

AxrResult AxrVulkanSceneData::initializeAllImageData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ImageData.empty()) {
        axrLogErrorLocation("Image data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeMissingTextureImageData();
    if (AXR_FAILED(axrResult)) {
        destroyAllImageData();
        return axrResult;
    }

    for (const auto& [imageName, image] : m_AssetCollection->getImages()) {
        axrResult = initializeImageData(image);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize image data.");
        destroyAllImageData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeImageData(const AxrImage& image) {
    const std::string imageName = image.getName();
    if (m_ImageData.contains(imageName)) return AXR_SUCCESS;

    const AxrVulkanImageData::Config imageDataConfig{
        .Name = imageName,
        .ImageHandle = &image,
        .PhysicalDevice = m_PhysicalDevice,
        .Device = m_Device,
        .GraphicsCommandPool = m_GraphicsCommandPool,
        .GraphicsQueue = m_GraphicsQueue,
        .DispatchHandle = m_DispatchHandle,
    };

    m_ImageData.insert(
        std::pair(
            imageName,
            AxrVulkanImageData(imageDataConfig)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeMissingTextureImageData() {
    if (!isThisGlobalSceneData()) return AXR_SUCCESS;

    AxrResult axrResult = AXR_SUCCESS;

    if (!m_MissingTextureImage.isLoaded()) {
        axrResult = axrEngineAssetCreateImage(
            axrEngineAssetGetImageName(AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE),
            AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE,
            m_MissingTextureImage
        );

        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to create missing texture image.");
            destroyAllImageData();
            return axrResult;
        }
    }

    axrResult = initializeImageData(m_MissingTextureImage);

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize missing texture image data.");
        destroyAllImageData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createImageData(AxrVulkanImageData& imageData) {
    const AxrResult axrResult = imageData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyImageData(imageData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyImageData(AxrVulkanImageData& imageData) {
    imageData.destroyData();
}

const AxrVulkanImageData* AxrVulkanSceneData::findImageData_shared(const std::string& name) const {
    const auto foundImageDataIt = m_ImageData.find(name);
    if (foundImageDataIt != m_ImageData.end()) {
        return &foundImageDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        const auto foundImageData = m_GlobalSceneData->findImageData_shared(name);

        if (foundImageData != nullptr) {
            return foundImageData;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::createAllImageSamplerData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ImageSamplerData.empty()) {
        axrLogErrorLocation("Image sampler data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllImageSamplerData();
    if (AXR_FAILED(axrResult)) {
        destroyAllImageSamplerData();
        return axrResult;
    }

    for (auto& [name, data] : m_ImageSamplerData) {
        axrResult = createImageSamplerData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllImageSamplerData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllImageSamplerData() {
    for (auto& [name, data] : m_ImageSamplerData) {
        destroyImageSamplerData(data);
    }
    m_ImageSamplerData.clear();
}

AxrResult AxrVulkanSceneData::initializeAllImageSamplerData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ImageSamplerData.empty()) {
        axrLogErrorLocation("Image sampler data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (const auto& [imageSamplerName, imageSampler] : m_AssetCollection->getImageSamplers()) {
        axrResult = initializeImageSamplerData(imageSampler);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize image sampler data.");
        destroyAllImageSamplerData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeImageSamplerData(const AxrImageSampler& imageSampler) {
    const std::string imageSamplerName = imageSampler.getName();
    if (m_ImageSamplerData.contains(imageSamplerName)) return AXR_SUCCESS;

    const AxrVulkanImageSamplerData::Config imageSamplerDataConfig{
        .Name = imageSamplerName,
        .ImageSamplerHandle = &imageSampler,
        .Device = m_Device,
        .MaxSamplerAnisotropy = m_MaxSamplerAnisotropy,
        .DispatchHandle = m_DispatchHandle,
    };

    m_ImageSamplerData.insert(
        std::pair(
            imageSamplerName,
            AxrVulkanImageSamplerData(imageSamplerDataConfig)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createImageSamplerData(AxrVulkanImageSamplerData& imageSamplerData) {
    const AxrResult axrResult = imageSamplerData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyImageSamplerData(imageSamplerData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyImageSamplerData(AxrVulkanImageSamplerData& imageSamplerData) {
    imageSamplerData.destroyData();
}

const AxrVulkanImageSamplerData* AxrVulkanSceneData::findImageSamplerData_shared(const std::string& name) const {
    const auto foundImageSamplerDataIt = m_ImageSamplerData.find(name);
    if (foundImageSamplerDataIt != m_ImageSamplerData.end()) {
        return &foundImageSamplerDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        const auto foundImageSamplerData = m_GlobalSceneData->findImageSamplerData_shared(name);

        if (foundImageSamplerData != nullptr) {
            return foundImageSamplerData;
        }
    }

    return nullptr;
}

const AxrShader* AxrVulkanSceneData::findShader_shared(const std::string& name) const {
    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return nullptr;
    }

    const AxrShader* foundShader = m_AssetCollection->findShader(name);

    if (foundShader != nullptr) {
        return foundShader;
    }

    if (m_GlobalSceneData != nullptr) {
        foundShader = m_GlobalSceneData->findShader_shared(name);

        if (foundShader != nullptr) {
            return foundShader;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::createAllMaterialLayoutData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialLayoutData.empty()) {
        axrLogErrorLocation("Material layout data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllMaterialLayoutData();
    if (AXR_FAILED(axrResult)) {
        destroyAllMaterialLayoutData();
        return axrResult;
    }

    for (auto& [name, data] : m_MaterialLayoutData) {
        axrResult = createMaterialLayoutData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllMaterialLayoutData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllMaterialLayoutData() {
    for (auto& [name, data] : m_MaterialLayoutData) {
        destroyMaterialLayoutData(data);
    }
    m_MaterialLayoutData.clear();
}

AxrResult AxrVulkanSceneData::initializeAllMaterialLayoutData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialLayoutData.empty()) {
        axrLogErrorLocation("Material layout data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (auto& [materialName, material] : m_AssetCollection->getMaterials()) {
        axrResult = initializeMaterialLayoutData(material);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize material layout data.");
        destroyAllMaterialLayoutData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeMaterialLayoutData(const AxrMaterial& material) {
    const std::string materialLayoutName = material.getMaterialLayoutName();
    if (m_MaterialLayoutData.contains(materialLayoutName)) return AXR_SUCCESS;

    const AxrShader* foundVertexShader = findShader_shared(material.getVertexShaderName());
    if (foundVertexShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find vertex shader named: {0}.",
            material.getVertexShaderName()
        );
        return AXR_ERROR;
    }

    const AxrShader* foundFragmentShader = findShader_shared(material.getFragmentShaderName());
    if (foundFragmentShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find fragment shader named: {0}.",
            material.getFragmentShaderName()
        );
        return AXR_ERROR;
    }

    const AxrVulkanMaterialLayoutData::Config materialLayoutDataConfig{
        .Name = materialLayoutName,
        .VertexShaderHandle = foundVertexShader,
        .FragmentShaderHandle = foundFragmentShader,
        .Device = m_Device,
        .DispatchHandle = m_DispatchHandle,
    };

    m_MaterialLayoutData.insert(
        std::pair(
            materialLayoutName,
            AxrVulkanMaterialLayoutData(materialLayoutDataConfig)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData) {
    const AxrResult axrResult = materialLayoutData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyMaterialLayoutData(materialLayoutData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData) {
    materialLayoutData.destroyData();
}

const AxrVulkanMaterialLayoutData* AxrVulkanSceneData::findMaterialLayoutData(const std::string& name) const {
    const auto foundMaterialLayoutData = m_MaterialLayoutData.find(name);
    if (foundMaterialLayoutData == m_MaterialLayoutData.end()) {
        return nullptr;
    }

    return &foundMaterialLayoutData->second;
}

AxrResult AxrVulkanSceneData::createAllMaterialData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialData.empty()) {
        axrLogErrorLocation("Material data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeAllMaterialData();
    if (AXR_FAILED(axrResult)) {
        destroyAllMaterialData();
        return axrResult;
    }

    for (auto& [name, data] : m_MaterialData) {
        axrResult = createMaterialData(data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllMaterialData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllMaterialData() {
    for (auto& [name, data] : m_MaterialData) {
        destroyMaterialData(data);
    }
    m_MaterialData.clear();
}

AxrResult AxrVulkanSceneData::initializeAllMaterialData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialData.empty()) {
        axrLogErrorLocation("Material data already exists.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (auto& [materialName, material] : m_AssetCollection->getMaterials()) {
        axrResult = initializeMaterialData(material);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to initialize material data.");
        destroyAllMaterialData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::initializeMaterialData(const AxrMaterial& material) {
    const std::string materialName = material.getName();
    if (m_MaterialData.contains(materialName)) return AXR_SUCCESS;

    const AxrShader* foundVertexShader = findShader_shared(material.getVertexShaderName());
    if (foundVertexShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find vertex shader named: {0}.",
            material.getVertexShaderName()
        );
        return AXR_ERROR;
    }

    const AxrShader* foundFragmentShader = findShader_shared(material.getFragmentShaderName());
    if (foundFragmentShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find fragment shader named: {0}.",
            material.getFragmentShaderName()
        );
        return AXR_ERROR;
    }

    const AxrVulkanMaterialLayoutData* foundMaterialLayoutData = findMaterialLayoutData(
        material.getMaterialLayoutName()
    );
    if (foundMaterialLayoutData == nullptr) {
        axrLogErrorLocation(
            "Failed to find material layout data for material: {0}.",
            material.getName()
        );
        return AXR_ERROR;
    }

    const AxrVulkanMaterialData::Config materialDataConfig{
        .Name = materialName,
        .VertexShaderHandle = foundVertexShader,
        .FragmentShaderHandle = foundFragmentShader,
        .MaterialHandle = &material,
        .MaterialLayoutData = foundMaterialLayoutData,
        .MaxFramesInFlight = m_MaxFramesInFlight,
        .Device = m_Device,
        .DispatchHandle = m_DispatchHandle,
    };

    m_MaterialData.insert(
        std::pair(
            materialName,
            AxrVulkanMaterialData(materialDataConfig)
        )
    );

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::createMaterialData(AxrVulkanMaterialData& materialData) {
    const AxrResult axrResult = materialData.createData();

    if (AXR_FAILED(axrResult)) {
        destroyMaterialData(materialData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyMaterialData(AxrVulkanMaterialData& materialData) {
    materialData.destroyData();
}

AxrResult AxrVulkanSceneData::createAllWindowMaterialData(const vk::RenderPass renderPass) {
    AxrResult axrResult = AXR_SUCCESS;

    for (auto& [name, data] : m_MaterialData) {
        axrResult = createWindowMaterialData(renderPass, data);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyAllWindowMaterialData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllWindowMaterialData() {
    for (auto& [name, data] : m_MaterialData) {
        destroyWindowMaterialData(data);
    }
}

// TODO: These function seems really pointless. just call materialData.createWindowData() and delete this function
//  Repeat for all of the same functions like this one
AxrResult AxrVulkanSceneData::createWindowMaterialData(
    const vk::RenderPass renderPass,
    AxrVulkanMaterialData& materialData
) {
    const AxrResult axrResult = materialData.createWindowData(renderPass);

    if (AXR_FAILED(axrResult)) {
        destroyWindowMaterialData(materialData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyWindowMaterialData(AxrVulkanMaterialData& materialData) {
    materialData.destroyWindowData();
}

const AxrVulkanMaterialData* AxrVulkanSceneData::findMaterialData_shared(const std::string& name) const {
    const auto foundMaterialDataIt = m_MaterialData.find(name);
    if (foundMaterialDataIt != m_MaterialData.end()) {
        return &foundMaterialDataIt->second;
    }

    if (m_GlobalSceneData != nullptr) {
        const auto foundMaterialData = m_GlobalSceneData->findMaterialData_shared(name);

        if (foundMaterialData != nullptr) {
            return foundMaterialData;
        }
    }

    return nullptr;
}

AxrResult AxrVulkanSceneData::writeAllWindowDescriptorSets() {
    AxrResult axrResult = AXR_SUCCESS;

    for (auto& [name, data] : m_MaterialData) {
        axrResult = writeDescriptorSets(AXR_PLATFORM_TYPE_WINDOW, AXR_SHADER_BUFFER_SCOPE_MATERIAL, this, data);

        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    // TODO: We probably don't need to reset everything if 1 fails.
    //  This will be a big task but we should go through everything and check if it should reset everything if 1 thing fails too.
    if (AXR_FAILED(axrResult)) {
        resetAllWindowDescriptorSets();
        return axrResult;
    }


    if (m_GlobalSceneData != nullptr) {
        axrResult = m_GlobalSceneData->writeAllSceneSpecificDescriptorSets(AXR_PLATFORM_TYPE_WINDOW, this);
        if (AXR_FAILED(axrResult)) {
            m_GlobalSceneData->resetAllWindowDescriptorSets();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::resetAllWindowDescriptorSets() {
    for (auto& [name, data] : m_MaterialData) {
        resetDescriptorSets(data);
    }
}

AxrResult AxrVulkanSceneData::writeAllSceneSpecificDescriptorSets(
    const AxrPlatformType platformType,
    const AxrVulkanSceneData* sceneData
) {
    // Nothing to do if we're not in the global scene data
    if (!isThisGlobalSceneData()) return AXR_SUCCESS;

    AxrResult axrResult = AXR_SUCCESS;

    for (auto& [name, data] : m_MaterialData) {
        axrResult = writeDescriptorSets(platformType, AXR_SHADER_BUFFER_SCOPE_SCENE, sceneData, data);

        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set all scene specific descriptor sets.");
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanSceneData::writeDescriptorSets(
    const AxrPlatformType platformType,
    const AxrShaderBufferScopeEnum bufferScope,
    const AxrVulkanSceneData* sceneData,
    AxrVulkanMaterialData& materialData
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!isPlatformLoaded(platformType)) {
        // The platform hasn't loaded so nothing to do
        return AXR_SUCCESS;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch Handle is null.");
        return AXR_ERROR;
    }

    if (sceneData == nullptr) {
        axrLogErrorLocation("Scene data is null.");
        return AXR_ERROR;
    }

    const AxrVulkanMaterialLayoutData* materialLayoutData = materialData.getMaterialLayoutData();
    if (materialLayoutData == nullptr) {
        axrLogErrorLocation("Material layout data is null.");
        return AXR_ERROR;
    }

    const AxrMaterial* material = materialData.getMaterial();
    if (material == nullptr) {
        axrLogErrorLocation("Material is null.");
        return AXR_ERROR;
    }

    const std::vector<vk::DescriptorSet>& descriptorSets = materialData.getWindowDescriptorSets();
    if (descriptorSets.empty()) {
        axrLogErrorLocation("Descriptor sets are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    const auto& descriptorSetItemLocations = materialLayoutData->getDescriptorSetItemLocations();
    std::vector<vk::WriteDescriptorSet> descriptorWrites;
    std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos;
    std::vector<vk::DescriptorImageInfo> descriptorImageInfos;

    // Make sure there's enough space for all these vectors. We don't want the 'info' vectors to resize and change
    // their pointers
    descriptorWrites.reserve(descriptorSetItemLocations.size() + m_MaxFramesInFlight);
    descriptorBufferInfos.reserve(descriptorSetItemLocations.size() + m_MaxFramesInFlight);
    descriptorImageInfos.reserve(descriptorSetItemLocations.size() + m_MaxFramesInFlight);

    for (auto descriptorSetItemLocation : descriptorSetItemLocations) {
        if (descriptorSetItemLocation.DescriptorType == vk::DescriptorType::eUniformBuffer) {
            const AxrShaderUniformBufferLinkConst_T uniformBuffer = material->findShaderUniformBuffer(
                descriptorSetItemLocation.ShaderBinding
            );
            if (uniformBuffer == nullptr) {
                axrLogErrorLocation(
                    "Failed to get uniform buffer at binding: {0}.",
                    descriptorSetItemLocation.ShaderBinding
                );
                axrResult = AXR_ERROR;
                break;
            }

            if (axrEngineAssetIsBufferNameReserved(uniformBuffer->BufferName) &&
                axrEngineAssetGetBufferScope(uniformBuffer->BufferName) != bufferScope) {
                // If the buffer's scope doesn't match the current scope we are writing for, we skip it.
                // We also check if it's an engine asset because they're the only buffers with a scope.
                continue;
            }

            for (uint32_t frameIndex = 0; frameIndex < m_MaxFramesInFlight; ++frameIndex) {
                const AxrVulkanUniformBufferData* foundUniformBufferData = sceneData->findUniformBufferData_shared(
                    uniformBuffer->BufferName,
                    platformType
                );

                if (foundUniformBufferData == nullptr) {
                    axrLogErrorLocation("Failed to find uniform buffer named: {0}.", uniformBuffer->BufferName);
                    axrResult = AXR_ERROR;
                    break;
                }

                descriptorBufferInfos.emplace_back(
                    foundUniformBufferData->getBuffer(frameIndex).getBuffer(),
                    0,
                    foundUniformBufferData->getBufferSize()
                );

                descriptorWrites.emplace_back(
                    descriptorSets[frameIndex],
                    descriptorSetItemLocation.ItemIndex,
                    0,
                    1,
                    descriptorSetItemLocation.DescriptorType,
                    nullptr,
                    &descriptorBufferInfos.back(),
                    nullptr
                );
            }
        } else if (descriptorSetItemLocation.DescriptorType == vk::DescriptorType::eCombinedImageSampler) {
            const AxrShaderImageSamplerBufferLinkConst_T imageSamplerBuffer = material->findShaderImageSamplerBuffer(
                descriptorSetItemLocation.ShaderBinding
            );
            if (imageSamplerBuffer == nullptr) {
                axrLogErrorLocation(
                    "Failed to get image sampler buffer at binding: {0}.",
                    descriptorSetItemLocation.ShaderBinding
                );
                axrResult = AXR_ERROR;
                break;
            }

            const AxrVulkanImageData* foundImageData = sceneData->findImageData_shared(imageSamplerBuffer->ImageName);

            if (foundImageData == nullptr) {
                // If image data wasn't found, use the "Missing Texture" image
                foundImageData = sceneData->findImageData_shared(
                    axrEngineAssetGetImageName(AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE)
                );

                if (foundImageData == nullptr) {
                    axrLogErrorLocation("Failed to find image named: {0}.", imageSamplerBuffer->ImageName);
                    axrResult = AXR_ERROR;
                    break;
                }

                // TODO: When we use the 'missing texture', make sure we use these sampler options NEAREST and REPEAT. otherwise it looks weird
            }

            const AxrVulkanImageSamplerData* foundImageSamplerData = sceneData->findImageSamplerData_shared(
                imageSamplerBuffer->SamplerName
            );

            if (foundImageSamplerData == nullptr) {
                axrLogErrorLocation("Failed to find image sampler named: {0}.", imageSamplerBuffer->SamplerName);
                axrResult = AXR_ERROR;
                break;
            }

            descriptorImageInfos.emplace_back(
                foundImageSamplerData->getSampler(),
                foundImageData->getImageView(),
                vk::ImageLayout::eShaderReadOnlyOptimal
            );

            for (uint32_t frameIndex = 0; frameIndex < m_MaxFramesInFlight; ++frameIndex) {
                descriptorWrites.emplace_back(
                    descriptorSets[frameIndex],
                    descriptorSetItemLocation.ItemIndex,
                    0,
                    1,
                    descriptorSetItemLocation.DescriptorType,
                    &descriptorImageInfos.back(),
                    nullptr,
                    nullptr
                );
            }
        }

        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    // TODO: We probably don't need to reset everything if 1 fails.
    //  This will be a big task but we should go through everything and check if it should reset everything if 1 thing fails too.
    if (AXR_FAILED(axrResult)) {
        resetDescriptorSets(materialData);
        return axrResult;
    }

    m_Device.updateDescriptorSets(
        static_cast<uint32_t>(descriptorWrites.size()),
        descriptorWrites.data(),
        0,
        nullptr,
        *m_DispatchHandle
    );

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::resetDescriptorSets(AxrVulkanMaterialData& materialData) const {
    materialData.resetWindowDescriptorSets();
}

AxrResult AxrVulkanSceneData::createAllMaterialsForRendering() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialsForRendering.empty()) {
        axrLogErrorLocation("Materials for rendering already exist.");
        return AXR_ERROR;
    }

    // We don't necessarily need a registry handle. The global assets definitely won't have one.
    if (m_EcsRegistryHandle == nullptr) {
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    for (const auto [entity, transformComponent, modelComponent] :
         m_EcsRegistryHandle->view<AxrTransformComponent, AxrModelComponent>().each()) {
        axrResult = addMaterialForRendering(transformComponent, modelComponent, m_MaterialsForRendering);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create materials for rendering.");
        destroyAllMaterialLayoutData();
        return axrResult;
    }

    // TODO: create callback anytime a new AxrTransformComponent and AxrModelComponent combo gets added.

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyAllMaterialsForRendering() {
    // m_MaterialsForRendering only contain references to objects so we don't need to explicitly clean up any objects
    m_MaterialsForRendering.clear();
}

AxrResult AxrVulkanSceneData::addMaterialForRendering(
    const AxrTransformComponent& transformComponent,
    const AxrModelComponent& modelComponent,
    std::unordered_map<std::string, MaterialForRendering>& materialsForRendering
) const {
    const AxrVulkanModelData* foundModelData = findModelData_shared(modelComponent.ModelName);
    if (foundModelData == nullptr) {
        axrLogErrorLocation("Failed to find model data for model: {0}.", modelComponent.ModelName);
    }

    for (uint32_t i = 0; i < modelComponent.MeshCount; ++i) {
        AxrModelComponent::Mesh& currentMesh = modelComponent.Meshes[i];

        const AxrVulkanMaterialData* foundMaterialData = findMaterialData_shared(currentMesh.MaterialName);
        if (foundMaterialData == nullptr) {
            axrLogErrorLocation("Failed to find material data for material: {0}.", currentMesh.MaterialName);
            continue;
        }

        auto foundMaterialForRendering = materialsForRendering.find(currentMesh.MaterialName);
        const vk::ShaderStageFlags& pushConstantStageFlags = foundMaterialData->getMaterialLayoutData()
                                                                              ->getPushConstantShaderStages();

        auto meshForRendering = MeshForRendering{
            .Buffer = foundModelData->getMeshBuffer(i),
            .BufferIndicesOffset = foundModelData->getMeshBufferIndicesOffset(i),
            .BufferVerticesOffset = foundModelData->getMeshBufferVerticesOffset(i),
            .IndexCount = foundModelData->getMeshIndexCount(i),
            .PushConstant = axrStringIsEmpty(modelComponent.PushConstantBufferName) ||
                            pushConstantStageFlags == static_cast<vk::ShaderStageFlagBits>(0)
                                ? PushConstantForRendering{}
                                : PushConstantForRendering{
                                    .ShaderStages = &pushConstantStageFlags,
                                    .BufferName = modelComponent.PushConstantBufferName,
                                    .TransformComponent = &transformComponent,
                                },
        };

        if (foundMaterialForRendering == materialsForRendering.end()) {
            const std::string& materialPushConstantBufferName = foundMaterialData->getPushConstantBufferName();

            materialsForRendering.insert(
                std::pair(
                    currentMesh.MaterialName,
                    MaterialForRendering{
                        .PipelineLayout = foundMaterialData->getMaterialLayoutData()->getPipelineLayout(),
                        .WindowPipeline = foundMaterialData->getWindowPipeline(),
                        .WindowDescriptorSets = foundMaterialData->getWindowDescriptorSets(),
                        .PushConstant = materialPushConstantBufferName.empty() ||
                                        pushConstantStageFlags == static_cast<vk::ShaderStageFlagBits>(0)
                                            ? PushConstantForRendering{}
                                            : PushConstantForRendering{
                                                .ShaderStages = &pushConstantStageFlags,
                                                .BufferName = materialPushConstantBufferName.c_str(),
                                                .TransformComponent = &transformComponent,
                                            },
                        .Meshes = {meshForRendering}
                    }
                )
            );
        } else {
            foundMaterialForRendering->second.Meshes.push_back(meshForRendering);
        }
    }

    return AXR_SUCCESS;
}

#endif
