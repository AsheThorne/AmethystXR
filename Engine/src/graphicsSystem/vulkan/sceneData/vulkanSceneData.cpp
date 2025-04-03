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
    m_AssetCollection(config.AssetCollection),
    m_SharedVulkanSceneData(config.SharedVulkanSceneData),
    m_Device(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr) {
}

AxrVulkanSceneData::~AxrVulkanSceneData() {
    unloadScene();
    resetSetup();
}

// ---- Public Functions ----

AxrResult AxrVulkanSceneData::setup(const SetupConfig& config) {
    if (isSetup()) {
        axrLogErrorLocation("Vulkan scene is already set up.");
        return AXR_ERROR;
    }

    if (config.Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (config.DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch Handle is null.");
        return AXR_ERROR;
    }

    m_Device = config.Device;
    m_DispatchHandle = config.DispatchHandle;

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::resetSetup() {
    unloadScene();

    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
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

    axrResult = createAllMaterialLayoutData();
    if (AXR_FAILED(axrResult)) {
        unloadScene();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadScene() {
    // TODO: Unload OpenXR data
    unloadWindowData();

    destroyAllMaterialLayoutData();
    if (m_AssetCollection != nullptr) {
        m_AssetCollection->unloadAssets();
    }
}

AxrResult AxrVulkanSceneData::loadWindowData() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createAllWindowMaterialLayoutData();
    if (AXR_FAILED(axrResult)) {
        unloadWindowData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::unloadWindowData() {
    destroyAllWindowMaterialLayoutData();
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

bool AxrVulkanSceneData::isSetup() {
    return m_Device != VK_NULL_HANDLE && m_DispatchHandle != nullptr;
}

AxrResult AxrVulkanSceneData::createAllMaterialLayoutData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialLayoutData.empty()) {
        axrLogErrorLocation("Material layout data already exist.");
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
        axrLogErrorLocation("Material layout data already exist.");
        return AXR_ERROR;
    }

    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    for (auto& [materialName, material] : m_AssetCollection->getMaterials()) {
        initializeMaterialLayoutData(material);
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::initializeMaterialLayoutData(const AxrMaterial& material) {
    const std::string materialLayoutName = material.getMaterialLayoutName();

    if (m_MaterialLayoutData.contains(materialLayoutName)) return;

    const AxrVulkanMaterialLayoutData::Config materialLayoutDataConfig{
        .Name = materialLayoutName,
        .VertexShaderName = material.getVertexShaderName(),
        .FragmentShaderName = material.getFragmentShaderName(),
        .Device = m_Device,
        .DispatchHandle = m_DispatchHandle,
    };

    m_MaterialLayoutData.insert(
        std::pair(
            materialLayoutName,
            AxrVulkanMaterialLayoutData(materialLayoutDataConfig)
        )
    );
}

AxrResult AxrVulkanSceneData::createMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData) {
    const AxrShader* foundVertexShader = findShader_shared(materialLayoutData.getVertexShaderName());
    if (foundVertexShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find vertex shader named: {0}.",
            materialLayoutData.getVertexShaderName()
        );
        return AXR_ERROR;
    }

    const AxrShader* foundFragmentShader = findShader_shared(materialLayoutData.getFragmentShaderName());
    if (foundFragmentShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find fragment shader named: {0}.",
            materialLayoutData.getFragmentShaderName()
        );
        return AXR_ERROR;
    }

    const AxrResult axrResult = materialLayoutData.createData(*foundVertexShader, *foundFragmentShader);

    if (AXR_FAILED(axrResult)) {
        destroyMaterialLayoutData(materialLayoutData);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneData::destroyMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData) {
    materialLayoutData.destroyData();
}

AxrResult AxrVulkanSceneData::createAllWindowMaterialLayoutData() {
    // TODO...
    return AXR_ERROR;
}

void AxrVulkanSceneData::destroyAllWindowMaterialLayoutData() {
    // TODO...
}

#endif
