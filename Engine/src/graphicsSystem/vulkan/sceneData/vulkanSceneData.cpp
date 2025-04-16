#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSceneData.hpp"
#include "axr/logger.h"
#include "../../../assets/assetCollection.hpp"
#include "../../../assets/material.hpp"
#include "../vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanSceneData::AxrVulkanSceneData(const Config& config):
    m_SceneName(config.SceneName),
    m_AssetCollection(config.AssetCollection),
    m_EcsRegistryHandle(config.EcsRegistryHandle),
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
    // TODO: See if we can wait for all the scene specific fences to be finished instead of doing this.
    const vk::Result vkResult = m_Device.waitIdle(*m_DispatchHandle);
    axrLogVkResult(vkResult, "m_Device.waitIdle");

    destroyAllWindowMaterialData();
}

const std::unordered_map<std::string, AxrVulkanSceneData::MaterialForRendering>&
AxrVulkanSceneData::getMaterialsForRendering() const {
    return m_MaterialsForRendering;
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

const AxrVulkanModelData* AxrVulkanSceneData::findModelData_shared(const std::string& name) const {
    const auto foundModelDataIt = m_ModelData.find(name);
    if (foundModelDataIt != m_ModelData.end()) {
        return &foundModelDataIt->second;
    }

    if (m_SharedVulkanSceneData != nullptr) {
        const auto foundModelData = m_SharedVulkanSceneData->findModelData_shared(name);

        if (foundModelData != nullptr) {
            return foundModelData;
        }
    }

    axrLogErrorLocation("Failed to find model data named: {0}.", name.c_str());
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

const AxrVulkanMaterialData* AxrVulkanSceneData::findMaterialData_shared(const std::string& name) const {
    const auto foundMaterialDataIt = m_MaterialData.find(name);
    if (foundMaterialDataIt != m_MaterialData.end()) {
        return &foundMaterialDataIt->second;
    }

    if (m_SharedVulkanSceneData != nullptr) {
        const auto foundMaterialData = m_SharedVulkanSceneData->findMaterialData_shared(name);

        if (foundMaterialData != nullptr) {
            return foundMaterialData;
        }
    }

    axrLogErrorLocation("Failed to find material data named: {0}.", name.c_str());
    return nullptr;
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

    for (uint32_t i = 0; i < modelComponent.MaterialNamesCount; ++i) {
        auto foundMaterialForRendering = materialsForRendering.find(modelComponent.MaterialNames[i]);

        if (foundMaterialForRendering != materialsForRendering.end()) {
            foundMaterialForRendering->second.Meshes.push_back(
                MeshForRendering{
                    .Buffer = foundModelData->getMeshBuffer(i),
                    .BufferIndicesOffset = foundModelData->getMeshBufferIndicesOffset(i),
                    .BufferVerticesOffset = foundModelData->getMeshBufferVerticesOffset(i),
                    .IndexCount = foundModelData->getMeshIndexCount(i),
                }
            );
        } else {
            const AxrVulkanMaterialData* foundMaterialData = findMaterialData_shared(modelComponent.MaterialNames[i]);
            if (foundMaterialData == nullptr) {
                axrLogErrorLocation("Failed to find material data for material: {0}.", modelComponent.MaterialNames[i]);
                continue;
            }

            materialsForRendering.insert(
                std::pair(
                    modelComponent.MaterialNames[i],
                    MaterialForRendering{
                        .PipelineLayout = foundMaterialData->getPipelineLayout(),
                        .WindowPipeline = foundMaterialData->getWindowPipeline(),
                        // TODO: PushConstantsForRendering
                        .PushConstantsForRendering = {},
                        .Meshes = {
                            MeshForRendering{
                                .Buffer = foundModelData->getMeshBuffer(i),
                                .BufferIndicesOffset = foundModelData->getMeshBufferIndicesOffset(i),
                                .BufferVerticesOffset = foundModelData->getMeshBufferVerticesOffset(i),
                                .IndexCount = foundModelData->getMeshIndexCount(i),
                            }
                        }
                    }
                )
            );
        }
    }

    return AXR_SUCCESS;
}

#endif
