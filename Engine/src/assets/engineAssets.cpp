// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "engineAssets.hpp"
#include "axr/logger.h"
#include "model.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <ranges>
#include <unordered_map>
#include <string>

// ---------------------------------------------------------------------------------- //
//                                 Global Variables                                   //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Shader Engine Assets
// ----------------------------------------- //

/// Engine asset shader names
const std::unordered_map EngineAssetShaderNames{
    std::pair(
        AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT,
        "AXR:ShaderDefaultVert"
    ),
    std::pair(
        AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG,
        "AXR:ShaderDefaultFrag"
    ),
};

// ----------------------------------------- //
// Buffer Engine Assets
// ----------------------------------------- //

/// Engine asset buffer properties
const std::unordered_map EngineAssetBufferProperties{
    std::pair(
        AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA,
        AxrEngineAssetBufferProperties{
            .Name = "AXR:UniformBufferSceneData",
            .Scope = AXR_SHADER_BUFFER_SCOPE_SCENE
        }
    ),
    std::pair(
        AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX,
        AxrEngineAssetBufferProperties{
            .Name = "AXR:PushConstantBufferModelMatrix",
            .Scope = AXR_SHADER_BUFFER_SCOPE_MODEL
        }
    ),
};

// ---------------------------------------------------------------------------------- //
//                                External Functions                                  //
// ---------------------------------------------------------------------------------- //

bool axrEngineAssetIsNameReserved(const char* name) {
    return axrEngineAssetIsShaderNameReserved(name) || axrEngineAssetIsBufferNameReserved(name);
}

const char* axrEngineAssetGetName(const AxrEngineAssetEnum engineAssetEnum) {
    if (axrEngineAssetIsShader(engineAssetEnum)) {
        return axrEngineAssetGetShaderName(engineAssetEnum);
    }
    if (axrEngineAssetIsUniformBuffer(engineAssetEnum)
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        || axrEngineAssetIsPushConstantBuffer(engineAssetEnum)
#endif
    ) {
        return axrEngineAssetGetBufferName(engineAssetEnum);
    }

    axrLogErrorLocation("Unknown engine asset enum.");
    return "";
}

// ----------------------------------------- //
// Buffer Engine Assets
// ----------------------------------------- //

uint64_t axrEngineAssetGetUniformBufferSize(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsUniformBuffer(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a uniform buffer.");
        return 0;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA: {
            return sizeof(AxrEngineAssetUniformBuffer_SceneData);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return 0;
        }
    }
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
uint32_t axrEngineAssetGetPushConstantBufferSize(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsPushConstantBuffer(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a push constant buffer.");
        return 0;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX: {
            return sizeof(AxrEngineAssetPushConstantBuffer_ModelMatrix);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return 0;
        }
    }
}
#endif

// ---------------------------------------------------------------------------------- //
//                                Internal Functions                                  //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Shader Engine Assets
// ----------------------------------------- //

bool axrEngineAssetIsShader(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_SHADER_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_SHADER_END;
}

const char* axrEngineAssetGetShaderName(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsShader(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a shader.");
        return "";
    }

    const auto foundEngineAssetIt = EngineAssetShaderNames.find(engineAssetEnum);
    if (foundEngineAssetIt == EngineAssetShaderNames.end()) {
        axrLogError("Failed to find name for engine asset: {0}.", static_cast<int>(engineAssetEnum));
        return "";
    }

    return foundEngineAssetIt->second;
}

bool axrEngineAssetIsShaderNameReserved(const char* name) {
    for (const auto& engineAssetName : EngineAssetShaderNames | std::views::values) {
        if (std::strcmp(engineAssetName, name) == 0) {
            return true;
        }
    }

    return false;
}

AxrResult axrEngineAssetCreateShader(const AxrEngineAssetEnum engineAssetEnum, AxrShader& shader) {
    if (!axrEngineAssetIsShader(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a shader.");
        return AXR_ERROR;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT: {
            return axrEngineAssetCreateShader_DefaultVert(shader);
        }
        case AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG: {
            return axrEngineAssetCreateShader_DefaultFrag(shader);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrEngineAssetCreateShader_DefaultVert(AxrShader& shader) {
    // TODO...
    return AXR_ERROR;
}

AxrResult axrEngineAssetCreateShader_DefaultFrag(AxrShader& shader) {
    // TODO...
    return AXR_ERROR;
}

// ----------------------------------------- //
// Buffer Engine Assets
// ----------------------------------------- //

bool axrEngineAssetIsUniformBuffer(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_UNIFORM_BUFFER_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_UNIFORM_BUFFER_END;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
bool axrEngineAssetIsPushConstantBuffer(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_END;
}
#endif

AxrShaderBufferScopeEnum axrEngineAssetGetBufferScope(const char* bufferName) {
    for (const auto& properties : EngineAssetBufferProperties | std::views::values) {
        if (std::strcmp(properties.Name, bufferName) == 0) {
            return properties.Scope;
        }
    }

    for (const auto& properties : EngineAssetBufferProperties | std::views::values) {
        if (std::strcmp(properties.Name, bufferName) == 0) {
            return properties.Scope;
        }
    }

    axrLogErrorLocation("Failed to find buffer engine asset named: {0}.", bufferName);
    return AXR_SHADER_BUFFER_SCOPE_UNDEFINED;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const char* axrEngineAssetGetBufferName(const AxrEngineAssetEnum engineAssetEnum) {
    // If the given engine asset is neither a uniform buffer nor a push constant buffer
    if (!(
        axrEngineAssetIsUniformBuffer(engineAssetEnum) ||
        axrEngineAssetIsPushConstantBuffer(engineAssetEnum)
    )) {
        axrLogErrorLocation("Engine asset is not a buffer.");
        return "";
    }

    const auto foundEngineAssetIt = EngineAssetBufferProperties.find(engineAssetEnum);
    if (foundEngineAssetIt == EngineAssetBufferProperties.end()) {
        axrLogError("Failed to find name for engine asset: {0}.", static_cast<int>(engineAssetEnum));
        return "";
    }

    return foundEngineAssetIt->second.Name;
}
#endif

const char* axrEngineAssetGetUniformBufferName(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsUniformBuffer(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset isn't a uniform buffer.");
        return "";
    }

    return axrEngineAssetGetBufferName(engineAssetEnum);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const char* axrEngineAssetGetPushConstantBufferName(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsPushConstantBuffer(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset isn't a push constant buffer.");
        return "";
    }

    return axrEngineAssetGetBufferName(engineAssetEnum);
}
#endif

bool axrEngineAssetIsBufferNameReserved(const char* name) {
    for (const auto& properties : EngineAssetBufferProperties | std::views::values) {
        if (std::strcmp(properties.Name, name) == 0) {
            return true;
        }
    }

    return false;
}

bool axrEngineAssetIsUniformBufferNameReserved(const char* name) {
    for (const auto& [engineAsset, properties] : EngineAssetBufferProperties) {
        if (axrEngineAssetIsUniformBuffer(engineAsset) && std::strcmp(properties.Name, name) == 0) {
            return true;
        }
    }

    return false;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
bool axrEngineAssetIsPushConstantBufferNameReserved(const char* name) {
    for (const auto& [engineAsset, properties] : EngineAssetBufferProperties) {
        if (axrEngineAssetIsPushConstantBuffer(engineAsset) && std::strcmp(properties.Name, name) == 0) {
            return true;
        }
    }

    return false;
}
#endif

// ----------------------------------------- //
// Material Engine Assets
// ----------------------------------------- //

AxrResult axrEngineAssetCreateMaterial_DefaultMaterial(
    const std::string& materialName,
    AxrEngineAssetMaterial_DefaultMaterial materialValues,
    AxrMaterial& material
) {
    // TODO...
    return AXR_ERROR;
}

// ----------------------------------------- //
// Model Engine Assets
// ----------------------------------------- //


bool axrEngineAssetIsModel(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_MODEL_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_MODEL_END;
}

AxrResult axrEngineAssetCreateModel(const std::string& modelName, const AxrEngineAssetEnum engineAssetEnum, AxrModel& model) {
    if (!axrEngineAssetIsModel(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a model.");
        return AXR_ERROR;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_MODEL_TRIANGLE: {
            return axrEngineAssetCreateModel_Triangle(modelName, model);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown model engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrEngineAssetCreateModel_Triangle(const std::string& modelName, AxrModel& model) {
    std::vector<AxrVertex> vertices{
        AxrVertex{
            .Position = {0.0f, 0.866f, 0.0f},
            .Color = {1.0f, 0.0f, 0.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.0f, 0.0f},
            .Color = {0.0f, 1.0f, 0.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.0f, 0.0f},
            .Color = {0.0f, 0.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
    };

    std::vector<uint32_t> indices{
        0, 1, 2,
    };

    AxrMesh mesh{
        .VerticesCount = static_cast<uint32_t>(vertices.size()),
        .Vertices = vertices.data(),
        .IndicesCount = static_cast<uint32_t>(indices.size()),
        .Indices = indices.data(),
    };

    const AxrModelConfig modelConfig{
        .Name = modelName.c_str(),
        .FilePath = "",
        .MeshesCount = 1,
        .Meshes = &mesh
    };

    model = AxrModel(modelConfig);

    return AXR_SUCCESS;
}
