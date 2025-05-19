// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "bufferEngineAssets.hpp"
#include "axr/assets.h"
#include "axr/logger.h"

// ----------------------------------------- //
// Global Variables
// ----------------------------------------- //

/// Uniform buffer engine asset names mapped to their enum
const std::unordered_map UniformBufferEngineAssetProperties{
    std::pair(
        AXR_UNIFORM_BUFFER_ENGINE_ASSET_SCENE_DATA,
        AxrShaderEngineAssetProperties{
            .Name = "AXR:UniformBufferSceneData",
            .Scope = AXR_SHADER_BUFFER_SCOPE_SCENE
        }
    ),
};

/// Push constants buffer engine asset names mapped to their enum
const std::unordered_map PushConstantsBufferEngineAssetProperties{
    std::pair(
        AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX,
        AxrShaderEngineAssetProperties{
            .Name = "AXR:PushConstantsBufferModelMatrix",
            .Scope = AXR_SHADER_BUFFER_SCOPE_MODEL
        }
    ),
};

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrIsBufferNameReserved(const char* name) {
    return axrIsUniformBufferNameReserved(name) || axrIsPushConstantsBufferNameReserved(name);
}

bool axrIsUniformBufferNameReserved(const char* name) {
    for (const auto& [engineAssetEnum, properties] : UniformBufferEngineAssetProperties) {
        if (std::strcmp(properties.Name, name) == 0) {
            return true;
        }
    }

    return false;
}

const char* axrGetUniformBufferEngineAssetName(const AxrUniformBufferEngineAssetEnum engineAssetEnum) {
    const auto foundUniformBufferEngineAssetIt = UniformBufferEngineAssetProperties.find(engineAssetEnum);
    if (foundUniformBufferEngineAssetIt == UniformBufferEngineAssetProperties.end()) {
        axrLogError("Failed to find uniform buffer engine asset.");
        return "";
    }

    return foundUniformBufferEngineAssetIt->second.Name;
}

uint64_t axrGetUniformBufferEngineAssetDataSize(const AxrUniformBufferEngineAssetEnum engineAssetEnum) {
    switch (engineAssetEnum) {
        case AXR_UNIFORM_BUFFER_ENGINE_ASSET_SCENE_DATA: {
            return sizeof(AxrUniformBufferEngineAsset_SceneData);
        }
        case AXR_UNIFORM_BUFFER_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return 0;
        }
    }
}

bool axrIsPushConstantsBufferNameReserved(const char* name) {
    for (const auto& [engineAssetEnum, properties] : PushConstantsBufferEngineAssetProperties) {
        if (std::strcmp(properties.Name, name) == 0) {
            return true;
        }
    }

    return false;
}

const char* axrGetPushConstantsBufferEngineAssetName(const AxrPushConstantsBufferEngineAssetEnum engineAssetEnum) {
    const auto foundPushConstantsBufferEngineAssetIt = PushConstantsBufferEngineAssetProperties.find(engineAssetEnum);
    if (foundPushConstantsBufferEngineAssetIt == PushConstantsBufferEngineAssetProperties.end()) {
        axrLogError("Failed to find push constants buffer engine asset.");
        return "";
    }

    return foundPushConstantsBufferEngineAssetIt->second.Name;
}

uint32_t axrGetPushConstantsBufferEngineAssetDataSize(const AxrPushConstantsBufferEngineAssetEnum engineAssetEnum) {
    switch (engineAssetEnum) {
        case AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX: {
            return sizeof(AxrPushConstantsBufferEngineAsset_ModelMatrix);
        }
        case AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return 0;
        }
    }
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

AxrShaderBufferScopeEnum axrGetBufferEngineAssetScope(const char* bufferName) {
    for (const auto& [engineAssetEnum, properties] : UniformBufferEngineAssetProperties) {
        if (std::strcmp(properties.Name, bufferName) == 0) {
            return properties.Scope;
        }
    }

    for (const auto& [engineAssetEnum, properties] : PushConstantsBufferEngineAssetProperties) {
        if (std::strcmp(properties.Name, bufferName) == 0) {
            return properties.Scope;
        }
    }

    axrLogErrorLocation("Failed to find buffer engine asset named: {0}.", bufferName);
    return AXR_SHADER_BUFFER_SCOPE_UNDEFINED;
}
