// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "axr/logger.h"

// ----------------------------------------- //
// Global Variables
// ----------------------------------------- //

/// Uniform buffer engine asset names mapped to their enum
const std::unordered_map<AxrUniformBufferEngineAssetEnum, const char*> UniformBufferEngineAssetNames{
    std::pair(
        AXR_UNIFORM_BUFFER_ENGINE_ASSET_VIEW_PROJ_MATRICES,
        "AXR:UniformBufferViewProjMatrices"
    ),
};

/// Push constants buffer engine asset names mapped to their enum
const std::unordered_map<AxrPushConstantsBufferEngineAssetEnum, const char*> PushConstantsBufferEngineAssetNames{
    std::pair(
        AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX,
        "AXR:PushConstantsBufferModelMatrix"
    ),
};

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrIsUniformBufferNameReserved(const char* name) {
    for (const auto& [uniformBufferAssetEnum, uniformBufferName] : UniformBufferEngineAssetNames) {
        if (std::strcmp(uniformBufferName, name) == 0) {
            return true;
        }
    }

    return false;
}

const char* axrGetUniformBufferEngineAssetName(const AxrUniformBufferEngineAssetEnum engineAssetEnum) {
    const auto foundUniformBufferEngineAssetIt = UniformBufferEngineAssetNames.find(engineAssetEnum);
    if (foundUniformBufferEngineAssetIt == UniformBufferEngineAssetNames.end()) {
        axrLogError("Failed to find uniform buffer engine asset.");
        return "";
    }

    return foundUniformBufferEngineAssetIt->second;
}

bool axrIsPushConstantsBufferNameReserved(const char* name) {
    for (const auto& [pushConstantsBufferAssetEnum, pushConstantsBufferName] : PushConstantsBufferEngineAssetNames) {
        if (std::strcmp(pushConstantsBufferName, name) == 0) {
            return true;
        }
    }

    return false;
}

const char* axrGetPushConstantsBufferEngineAssetName(const AxrPushConstantsBufferEngineAssetEnum engineAssetEnum) {
    const auto foundPushConstantsBufferEngineAssetIt = PushConstantsBufferEngineAssetNames.find(engineAssetEnum);
    if (foundPushConstantsBufferEngineAssetIt == PushConstantsBufferEngineAssetNames.end()) {
        axrLogError("Failed to find push constants buffer engine asset.");
        return "";
    }

    return foundPushConstantsBufferEngineAssetIt->second;
}
