#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSceneData.hpp"
#include "axr/logger.h"
#include "../../../assets/assetCollection.hpp"
#include "../../../assets/material.hpp"

// ---- Special Functions ----

AxrVulkanSceneData::AxrVulkanSceneData(const Config& config):
    m_SceneName(config.SceneName),
    m_AssetCollection(config.AssetCollection),
    m_SharedVulkanSceneData(config.SharedVulkanSceneData),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle) {
}

AxrVulkanSceneData::~AxrVulkanSceneData() {
    unloadScene();
}

// ---- Public Functions ----

const char* AxrVulkanSceneData::getSceneName() const {
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

    axrResult = createAllModelData();
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

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadScene() {
    // TODO: Unload OpenXR data
    unloadWindowData();

    destroyAllMaterialData();
    destroyAllMaterialLayoutData();
    destroyAllModelData();
    if (m_AssetCollection != nullptr) {
        m_AssetCollection->unloadAssets();
    }
}

AxrResult AxrVulkanSceneData::loadWindowData(const vk::RenderPass renderPass) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createAllWindowMaterialData(renderPass);
    if (AXR_FAILED(axrResult)) {
        unloadWindowData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadWindowData() {
    destroyAllWindowMaterialData();
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

    if (m_SharedVulkanSceneData != nullptr) {
        foundShader = m_SharedVulkanSceneData->findShader_shared(name);

        if (foundShader != nullptr) {
            return foundShader;
        }
    }

    axrLogErrorLocation("Failed to find shader named: {0}.", name.c_str());
    return nullptr;
}

// ---- Private Functions ----

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
        .PipelineLayout = foundMaterialLayoutData->getPipelineLayout(),
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

#endif
