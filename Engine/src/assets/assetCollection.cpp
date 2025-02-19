// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "assetCollection.hpp"
#include "axr/logger.h"
#include "shaderEngineAssets.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

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

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrAssetCollection::AxrAssetCollection() = default;

AxrAssetCollection::~AxrAssetCollection() = default;

// ---- Public Functions ----

AxrResult AxrAssetCollection::createShader(AxrShaderConfig shaderConfig) {
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

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrShader shader;
    const AxrResult axrResult = axrCreateEngineAssetShader(engineAssetEnum, shader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to get shader engine asset config.");
        return axrResult;
    }

    const auto insertResult = m_Shaders.insert(std::pair(shader.getName(), std::move(shader)));
    if (!insertResult.second) {
        // If the insertion failed
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}
