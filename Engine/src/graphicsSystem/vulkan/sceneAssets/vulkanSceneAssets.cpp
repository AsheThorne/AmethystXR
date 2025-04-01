#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSceneAssets.hpp"
#include "axr/logger.h"
#include "../../../assets/assetCollection.hpp"
#include "../../../assets/material.hpp"

// ---- Special Functions ----

AxrVulkanSceneAssets::AxrVulkanSceneAssets(const Config& config):
    m_AssetCollection(config.AssetCollection),
    m_SharedVulkanSceneAssets(config.SharedVulkanSceneAssets),
    m_Device(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr) {
}

AxrVulkanSceneAssets::~AxrVulkanSceneAssets() {
    unloadAssets();
    resetSetup();
}

// ---- Public Functions ----

AxrResult AxrVulkanSceneAssets::setup(const SetupConfig& config) {
    if (isSetup()) {
        axrLogErrorLocation("Vulkan scene assets are already set up.");
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

void AxrVulkanSceneAssets::resetSetup() {
    unloadAssets();

    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanSceneAssets::loadAssets() {
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
        unloadAssets();
        return axrResult;
    }

    axrResult = loadMaterialLayoutAssets();
    if (AXR_FAILED(axrResult)) {
        unloadAssets();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneAssets::unloadAssets() {
    // TODO: Unload OpenXR assets
    unloadWindowAssets();

    unloadMaterialLayoutAssets();
    if (m_AssetCollection != nullptr) {
        m_AssetCollection->unloadAssets();
    }
}

AxrResult AxrVulkanSceneAssets::loadWindowAssets() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = loadWindowMaterialLayoutAssets();
    if (AXR_FAILED(axrResult)) {
        unloadWindowAssets();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneAssets::unloadWindowAssets() {
    unloadWindowMaterialLayoutAssets();
}

const AxrShader* AxrVulkanSceneAssets::findShader_shared(const std::string& name) const {
    if (m_AssetCollection == nullptr) {
        axrLogErrorLocation("Asset collection is null.");
        return nullptr;
    }

    const AxrShader* foundShader = m_AssetCollection->findShader(name);

    if (foundShader != nullptr) {
        return foundShader;
    }

    if (m_SharedVulkanSceneAssets != nullptr) {
        foundShader = m_SharedVulkanSceneAssets->findShader_shared(name);

        if (foundShader != nullptr) {
            return foundShader;
        }
    }

    axrLogErrorLocation("Failed to find shader named: {0}.", name.c_str());
    return nullptr;
}

// ---- Private Functions ----

bool AxrVulkanSceneAssets::isSetup() {
    return m_Device != VK_NULL_HANDLE && m_DispatchHandle != nullptr;
}

AxrResult AxrVulkanSceneAssets::loadMaterialLayoutAssets() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialLayoutAssets.empty()) {
        axrLogErrorLocation("Material layout assets already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = initializeMaterialLayoutAssets();
    if (AXR_FAILED(axrResult)) {
        unloadMaterialLayoutAssets();
        return axrResult;
    }

    for (auto& [materialLayoutName, materialLayoutAssets] : m_MaterialLayoutAssets) {
        axrResult = loadMaterialLayoutAsset(materialLayoutAssets);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        unloadMaterialLayoutAssets();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneAssets::unloadMaterialLayoutAssets() {
    for (auto& [materialLayoutName, materialLayoutAssets] : m_MaterialLayoutAssets) {
        unloadMaterialLayoutAsset(materialLayoutAssets);
    }
    m_MaterialLayoutAssets.clear();
}

AxrResult AxrVulkanSceneAssets::initializeMaterialLayoutAssets() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MaterialLayoutAssets.empty()) {
        axrLogErrorLocation("Material layout assets already exist.");
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
        initializeMaterialLayoutAsset(material);
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneAssets::initializeMaterialLayoutAsset(const AxrMaterial& material) {
    const std::string materialLayoutName = material.getMaterialLayoutName();

    if (m_MaterialLayoutAssets.contains(materialLayoutName)) return;

    const AxrVulkanMaterialLayoutAssets::Config materialLayoutAssetsConfig{
        .Name = materialLayoutName,
        .VertexShaderName = material.getVertexShaderName(),
        .FragmentShaderName = material.getFragmentShaderName(),
        .Device = m_Device,
        .DispatchHandle = m_DispatchHandle,
    };

    m_MaterialLayoutAssets.insert(
        std::pair(
            materialLayoutName,
            AxrVulkanMaterialLayoutAssets(materialLayoutAssetsConfig)
        )
    );
}

AxrResult AxrVulkanSceneAssets::loadMaterialLayoutAsset(AxrVulkanMaterialLayoutAssets& materialLayoutAssets) {
    const AxrShader* foundVertexShader = findShader_shared(materialLayoutAssets.getVertexShaderName());
    if (foundVertexShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find vertex shader named: {0}.",
            materialLayoutAssets.getVertexShaderName()
        );
        return AXR_ERROR;
    }

    const AxrShader* foundFragmentShader = findShader_shared(materialLayoutAssets.getFragmentShaderName());
    if (foundFragmentShader == nullptr) {
        axrLogErrorLocation(
            "Failed to find fragment shader named: {0}.",
            materialLayoutAssets.getFragmentShaderName()
        );
        return AXR_ERROR;
    }

    const AxrResult axrResult = materialLayoutAssets.createAssets(*foundVertexShader, *foundFragmentShader);

    if (AXR_FAILED(axrResult)) {
        unloadMaterialLayoutAsset(materialLayoutAssets);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanSceneAssets::unloadMaterialLayoutAsset(AxrVulkanMaterialLayoutAssets& materialLayoutAssets) {
    materialLayoutAssets.destroyAssets();
}

AxrResult AxrVulkanSceneAssets::loadWindowMaterialLayoutAssets() {
    // TODO...
    return AXR_ERROR;
}

void AxrVulkanSceneAssets::unloadWindowMaterialLayoutAssets() {
    // TODO...
}

#endif
