// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shaderEngineAssets.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// Global Variables
// ----------------------------------------- //

/// Shader engine asset names mapped to their enum
const std::unordered_map<AxrShaderEngineAssetEnum, const char*> ShaderEngineAssetNames{
    std::pair(
        AXR_SHADER_ENGINE_ASSET_DEFAULT_VERT,
        "AXR:ShaderDefaultVert"
    ),
    std::pair(
        AXR_SHADER_ENGINE_ASSET_DEFAULT_FRAG,
        "AXR:ShaderDefaultFrag"
    ),
};

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrIsShaderNameReserved(const char* name) {
    for (const auto& [shaderAssetEnum, shaderName] : ShaderEngineAssetNames) {
        if (std::strcmp(shaderName, name) == 0) {
            return true;
        }
    }

    return false;
}

const char* axrGetShaderEngineAssetName(const AxrShaderEngineAssetEnum engineAssetEnum) {
    const auto foundShaderEngineAssetIt = ShaderEngineAssetNames.find(engineAssetEnum);
    if (foundShaderEngineAssetIt == ShaderEngineAssetNames.end()) {
        axrLogError("Failed to find shader engine asset.");
        return "";
    }

    return foundShaderEngineAssetIt->second;
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

AxrResult axrCreateEngineAssetShader(const AxrShaderEngineAssetEnum engineAssetEnum, AxrShader& shader) {
    switch (engineAssetEnum) {
        case AXR_SHADER_ENGINE_ASSET_DEFAULT_VERT: {
            return axrCreateEngineAssetShader_DefaultVert(shader);
        }
        case AXR_SHADER_ENGINE_ASSET_DEFAULT_FRAG: {
            return axrCreateEngineAssetShader_DefaultFrag(shader);
        }
        case AXR_SHADER_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrCreateEngineAssetShader_DefaultVert(AxrShader& shader) {
    // TODO...
    return AXR_ERROR;
}

AxrResult axrCreateEngineAssetShader_DefaultFrag(AxrShader& shader) {
    // TODO...
    return AXR_ERROR;
}
