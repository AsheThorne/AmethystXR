// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "assetCollection.hpp"
#include "axr/logger.h"
#include "shaderEngineAssets.hpp"
#include "materialEngineAssets.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrAssetCollectionIsLoaded(const AxrAssetCollection_T assetCollection) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return false;
    }

    return assetCollection->isLoaded();
}

AxrResult axrAssetCollectionLoadAssets(
    const AxrAssetCollection_T assetCollection,
    const AxrGraphicsApiEnum graphicsApi
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->loadAssets(graphicsApi);
}

void axrAssetCollectionUnloadAssets(const AxrAssetCollection_T assetCollection) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return;
    }

    assetCollection->unloadAssets();
}

AxrResult axrAssetCollectionCreateShader(
    const AxrAssetCollection_T assetCollection,
    const AxrShaderConfig* shaderConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (shaderConfig == nullptr) {
        axrLogErrorLocation("`shaderConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createShader(*shaderConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetShader(
    const AxrAssetCollection_T assetCollection,
    const AxrShaderEngineAssetEnum engineAssetEnum
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createShader(engineAssetEnum);
}

AxrResult axrAssetCollectionCreateMaterial(
    const AxrAssetCollection_T assetCollection,
    const AxrMaterialConfig* materialConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (materialConfig == nullptr) {
        axrLogErrorLocation("`materialConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createMaterial(*materialConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
    const AxrAssetCollection_T assetCollection,
    const char* materialName,
    const AxrMaterialEngineAsset_DefaultMaterial materialValues
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createMaterial(materialName, materialValues);
}


// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrAssetCollection::AxrAssetCollection() = default;

AxrAssetCollection::~AxrAssetCollection() {
    cleanup();
}

// ---- Public Functions ----

void AxrAssetCollection::cleanup() {
    m_Shaders.clear();
    m_Materials.clear();
}

bool AxrAssetCollection::isLoaded() {
    for (auto& [shaderName, shader] : m_Shaders) {
        if (!shader.isLoaded()) {
            return false;
        }
    }

    return true;
}

AxrResult AxrAssetCollection::loadAssets(const AxrGraphicsApiEnum graphicsApi) {
    for (auto& [shaderName, shader] : m_Shaders) {
        const AxrResult axrResult = shader.loadFile(graphicsApi);
        if (AXR_FAILED(axrResult)) {
            unloadAssets();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrAssetCollection::unloadAssets() {
    for (auto& [shaderName, shader] : m_Shaders) {
        shader.unloadFile();
    }
}

AxrResult AxrAssetCollection::createShader(const AxrShaderConfig& shaderConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (axrIsShaderNameReserved(shaderConfig.Name)) {
        axrLogError("Unable to create shader. The shader name: {0} is reserved by the engine.", shaderConfig.Name);
        return AXR_ERROR;
    }

    if (m_Shaders.contains(shaderConfig.Name)) {
        axrLogError("Unable to create shader. A shader named: {0} already exists.", shaderConfig.Name);
        return AXR_ERROR;
    }

    if (!AxrShaderPropertiesRAII::isValid(shaderConfig.Properties)) {
        axrLogError("Unable to create shader. Shader properties are invalid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Shaders.insert(std::pair(shaderConfig.Name, AxrShader(shaderConfig)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    // TODO: Reload vulkan scene assets if they're already loaded.

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createShader(const AxrShaderEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    const char* shaderName = axrGetShaderEngineAssetName(engineAssetEnum);
    if (std::strcmp(shaderName, "") != 0) {
        axrLogError("Unable to create shader. Unknown shader engine asset.");
    }

    if (m_Shaders.contains(shaderName)) {
        axrLogError("Unable to create shader. A shader named: {0} already exists.", shaderName);
        return AXR_ERROR;
    }

    // TODO: Check that the shader is valid??

    AxrShader shader;
    const AxrResult axrResult = axrCreateEngineAssetShader(engineAssetEnum, shader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create shader engine asset.");
        return axrResult;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Shaders.insert(std::pair(shader.getName(), std::move(shader)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    // TODO: Reload vulkan scene assets if they're already loaded.

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createMaterial(const AxrMaterialConfig& materialConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Materials.contains(materialConfig.Name)) {
        axrLogError("Unable to create material. A material named: {0} already exists.", materialConfig.Name);
        return AXR_ERROR;
    }

    if (!AxrShaderValuesRAII::isValid(materialConfig.VertexShaderValues)) {
        axrLogError("Unable to create material. Vertex shader values are invalid.");
        return AXR_ERROR;
    }

    if (!AxrShaderValuesRAII::isValid(materialConfig.FragmentShaderValues)) {
        axrLogError("Unable to create material. Fragment shader values are invalid.");
        return AXR_ERROR;
    }

    // TODO: Check that the shaders this material uses have been registered

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Materials.insert(std::pair(materialConfig.Name, AxrMaterial(materialConfig)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    // TODO: Reload vulkan scene assets if they're already loaded.

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createMaterial(
    const char* materialName,
    const AxrMaterialEngineAsset_DefaultMaterial materialValues
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Materials.contains(materialName)) {
        axrLogError("Unable to create material. A material named: {0} already exists.", materialName);
        return AXR_ERROR;
    }

    // TODO: Check that the material is valid??

    AxrMaterial material;
    const AxrResult axrResult = axrCreateEngineAssetMaterial_DefaultMaterial(materialName, materialValues, material);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create material engine asset.");
        return axrResult;
    }

    if (!m_Shaders.contains(material.getVertexShaderName())) {
        axrLogError(
            "Unable to create material. The shader named: {0} doesn't exist.",
            material.getVertexShaderName()
        );
        return AXR_ERROR;
    }

    if (!m_Shaders.contains(material.getFragmentShaderName())) {
        axrLogError(
            "Unable to create material. The shader named: {0} doesn't exist.",
            material.getFragmentShaderName()
        );
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Materials.insert(std::pair(materialName, std::move(material)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    // TODO: Reload vulkan scene assets if they're already loaded.

    return AXR_SUCCESS;
}

const AxrShader* AxrAssetCollection::findShader(const std::string& name) {
    const auto foundShaderIterator = m_Shaders.find(name);
    if (foundShaderIterator == m_Shaders.end()) {
        return nullptr;
    }

    return &foundShaderIterator->second;
}

const std::unordered_map<std::string, AxrShader>& AxrAssetCollection::getShaders() {
    return m_Shaders;
}

const std::unordered_map<std::string, AxrMaterial>& AxrAssetCollection::getMaterials() {
    return m_Materials;
}
