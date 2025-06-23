// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "engineAssets.hpp"
#include "axr/logger.h"
#include "model.hpp"
#include "assetsUtils.hpp"
#include "shader.hpp"
#include "material.hpp"
#include "image.hpp"

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
const std::unordered_map EngineAssetBufferNames{
    std::pair(
        AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA,
        "AXR:UniformBufferSceneData"
    ),
    std::pair(
        AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX,
        "AXR:PushConstantBufferModelMatrix"
    ),
};

// ----------------------------------------- //
// Image Engine Assets
// ----------------------------------------- //

/// Engine asset image names
const std::unordered_map EngineAssetImageNames{
    // Only the missing texture has a name.
    // It's the only image that must be available for any scene.
    std::pair(
        AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE,
        "AXR:ImageMissingTexture"
    ),
};
// ---------------------------------------------------------------------------------- //
//                                External Functions                                  //
// ---------------------------------------------------------------------------------- //

bool axrEngineAssetIsNameReserved(const char* name) {
    return axrEngineAssetIsShaderNameReserved(name) ||
        axrEngineAssetIsBufferNameReserved(name) ||
        axrEngineAssetIsImageNameReserved(name);
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
    std::array vertexAttributes{
        AxrShaderVertexAttribute{
            .Type = AXR_SHADER_VERTEX_ATTRIBUTE_POSITION,
            .Binding = 0,
            .Location = 0,
        },
        AxrShaderVertexAttribute{
            .Type = AXR_SHADER_VERTEX_ATTRIBUTE_COLOR,
            .Binding = 0,
            .Location = 1,
        },
        AxrShaderVertexAttribute{
            .Type = AXR_SHADER_VERTEX_ATTRIBUTE_TEX_COORDS,
            .Binding = 0,
            .Location = 2,
        }
    };

    AxrShaderUniformBufferLayout sceneDataBufferLayout{
        .Binding = 0,
        .BufferSize = axrEngineAssetGetUniformBufferSize(AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA)
    };

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    AxrShaderPushConstantBufferLayout modelMatrixBufferLayout{
        .BufferSize = axrEngineAssetGetPushConstantBufferSize(AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX)
    };
#endif

    std::array bufferLayouts{
        reinterpret_cast<AxrShaderBufferLayout_T>(&sceneDataBufferLayout),
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        reinterpret_cast<AxrShaderBufferLayout_T>(&modelMatrixBufferLayout),
#endif
    };

    AxrVertexShaderProperties shaderProperties{
        .VertexAttributesCount = static_cast<uint32_t>(vertexAttributes.size()),
        .VertexAttributes = vertexAttributes.data(),
        .BufferLayoutsCount = static_cast<uint32_t>(bufferLayouts.size()),
        .BufferLayouts = bufferLayouts.data(),
    };

    const auto shaderPath = axrGetEngineAssetsDirectoryPath().append("shaders/shader.vert").generic_string();

    const AxrShaderConfig shaderConfig{
        .Name = axrEngineAssetGetShaderName(AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT),
        .FilePath = shaderPath.c_str(),
        .Properties = reinterpret_cast<AxrShaderProperties_T>(&shaderProperties)
    };

    if (!axrShaderConfigIsValid(&shaderConfig)) {
        return AXR_ERROR;
    }

    shader = AxrShader(shaderConfig);

    return AXR_SUCCESS;
}

AxrResult axrEngineAssetCreateShader_DefaultFrag(AxrShader& shader) {
    AxrShaderImageSamplerBufferLayout imageSamplerBufferLayout{
        .Binding = 1,
    };

    std::array bufferLayouts{
        reinterpret_cast<AxrShaderBufferLayout_T>(&imageSamplerBufferLayout),
    };

    AxrFragmentShaderProperties shaderProperties{
        .BufferLayoutsCount = static_cast<uint32_t>(bufferLayouts.size()),
        .BufferLayouts = bufferLayouts.data(),
    };

    const auto shaderPath = axrGetEngineAssetsDirectoryPath().append("shaders/shader.frag").generic_string();

    const AxrShaderConfig shaderConfig{
        .Name = axrEngineAssetGetShaderName(AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG),
        .FilePath = shaderPath.c_str(),
        .Properties = reinterpret_cast<AxrShaderProperties_T>(&shaderProperties)
    };

    if (!axrShaderConfigIsValid(&shaderConfig)) {
        return AXR_ERROR;
    }

    shader = AxrShader(shaderConfig);

    return AXR_SUCCESS;
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

const char* axrEngineAssetGetBufferName(const AxrEngineAssetEnum engineAssetEnum) {
    if (!(
        axrEngineAssetIsUniformBuffer(engineAssetEnum)
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        || axrEngineAssetIsPushConstantBuffer(engineAssetEnum)
#endif
    )) {
        axrLogErrorLocation("Engine asset is not a buffer.");
        return "";
    }

    const auto foundEngineAssetIt = EngineAssetBufferNames.find(engineAssetEnum);
    if (foundEngineAssetIt == EngineAssetBufferNames.end()) {
        axrLogError("Failed to find name for engine asset: {0}.", static_cast<int>(engineAssetEnum));
        return "";
    }

    return foundEngineAssetIt->second;
}

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
    for (const auto& bufferName : EngineAssetBufferNames | std::views::values) {
        if (std::strcmp(bufferName, name) == 0) {
            return true;
        }
    }

    return false;
}

bool axrEngineAssetIsUniformBufferNameReserved(const char* name) {
    for (const auto& [engineAsset, bufferName] : EngineAssetBufferNames) {
        if (axrEngineAssetIsUniformBuffer(engineAsset) && std::strcmp(bufferName, name) == 0) {
            return true;
        }
    }

    return false;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
bool axrEngineAssetIsPushConstantBufferNameReserved(const char* name) {
    for (const auto& [engineAsset, bufferName] : EngineAssetBufferNames) {
        if (axrEngineAssetIsPushConstantBuffer(engineAsset) && std::strcmp(bufferName, name) == 0) {
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
    const AxrEngineAssetMaterial_DefaultMaterial materialValues,
    AxrMaterial& material
) {
    AxrShaderUniformBufferLink sceneDataBufferLink{
        .Binding = 0,
        .BufferName = axrEngineAssetGetUniformBufferName(AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA)
    };

    std::array vertexBufferLinks{
        reinterpret_cast<AxrShaderBufferLink_T>(&sceneDataBufferLink),
    };

    AxrShaderValues vertexShaderValues{
        .BufferLinksCount = static_cast<uint32_t>(vertexBufferLinks.size()),
        .BufferLinks = vertexBufferLinks.data(),
    };

    AxrShaderImageSamplerBufferLink imageSamplerBufferLink{
        .Binding = 1,
        .ImageName = materialValues.ImageName,
        .SamplerName = materialValues.SamplerName,
    };

    std::array fragmentBufferLinks{
        reinterpret_cast<AxrShaderBufferLink_T>(&imageSamplerBufferLink),
    };

    AxrShaderValues fragmentShaderValues{
        .BufferLinksCount = static_cast<uint32_t>(fragmentBufferLinks.size()),
        .BufferLinks = fragmentBufferLinks.data()
    };

    const AxrMaterialConfig materialConfig{
        .Name = materialName.c_str(),
        .VertexShaderName = axrEngineAssetGetShaderName(AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT),
        .FragmentShaderName = axrEngineAssetGetShaderName(AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG),
        .PushConstantBufferName = "",
        .VertexShaderValues = &vertexShaderValues,
        .FragmentShaderValues = &fragmentShaderValues
    };

    if (!axrMaterialConfigIsValid(&materialConfig)) {
        return AXR_ERROR;
    }

    material = AxrMaterial(materialConfig);

    return AXR_SUCCESS;
}

// ----------------------------------------- //
// Model Engine Assets
// ----------------------------------------- //

bool axrEngineAssetIsModel(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_MODEL_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_MODEL_END;
}

AxrResult axrEngineAssetCreateModel(
    const std::string& modelName,
    const AxrEngineAssetEnum engineAssetEnum,
    AxrModel& model
) {
    if (!axrEngineAssetIsModel(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not a model.");
        return AXR_ERROR;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_MODEL_TRIANGLE: {
            return axrEngineAssetCreateModel_Triangle(modelName, model);
        }
        case AXR_ENGINE_ASSET_MODEL_SQUARE: {
            return axrEngineAssetCreateModel_Square(modelName, model);
        }
        case AXR_ENGINE_ASSET_MODEL_CUBE: {
            return axrEngineAssetCreateModel_Cube(modelName, model);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown model engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrEngineAssetCreateModel_Triangle(const std::string& modelName, AxrModel& model) {
    const AxrModelConfig modelConfig{
        .Name = modelName.c_str(),
        .FilePath = ""
    };
    model = AxrModel(modelConfig);

    std::vector<AxrVertex> vertices{
        AxrVertex{
            .Position = {0.0f, 0.866f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.5f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },
    };

    std::vector<uint32_t> indices{
        0, 1, 2,
    };

    AxrSubmesh submesh{
        .VerticesCount = static_cast<uint32_t>(vertices.size()),
        .Vertices = vertices.data(),
        .IndicesCount = static_cast<uint32_t>(indices.size()),
        .Indices = indices.data(),
    };

    const AxrMesh mesh{
        .SubmeshCount = 1,
        .Submeshes = &submesh,
    };

    const AxrResult axrResult = model.setData(1, &mesh);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult axrEngineAssetCreateModel_Square(const std::string& modelName, AxrModel& model) {
    const AxrModelConfig modelConfig{
        .Name = modelName.c_str(),
        .FilePath = ""
    };

    model = AxrModel(modelConfig);

    std::vector<AxrVertex> vertices{
        AxrVertex{
            .Position = {-0.5f, 1.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 1.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.0f, 0.0f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },
    };

    std::vector<uint32_t> indices{
        0, 1, 2,
        2, 3, 0,
    };

    AxrSubmesh submesh{
        .VerticesCount = static_cast<uint32_t>(vertices.size()),
        .Vertices = vertices.data(),
        .IndicesCount = static_cast<uint32_t>(indices.size()),
        .Indices = indices.data(),
    };

    const AxrMesh mesh{
        .SubmeshCount = 1,
        .Submeshes = &submesh,
    };

    const AxrResult axrResult = model.setData(1, &mesh);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult axrEngineAssetCreateModel_Cube(const std::string& modelName, AxrModel& model) {
    const AxrModelConfig modelConfig{
        .Name = modelName.c_str(),
        .FilePath = ""
    };

    model = AxrModel(modelConfig);

    std::vector<AxrVertex> vertices{
        /// Front face
        AxrVertex{
            .Position = {-0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },

        // Back face
        AxrVertex{
            .Position = {0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },

        // Top face
        AxrVertex{
            .Position = {-0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },

        // Bottom face
        AxrVertex{
            .Position = {0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },

        // Left face
        AxrVertex{
            .Position = {-0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {-0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },

        // Right face
        AxrVertex{
            .Position = {0.5f, 0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, 0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 0.0f},
        },
        AxrVertex{
            .Position = {0.5f, -0.5f, -0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {1.0f, 1.0f},
        },
        AxrVertex{
            .Position = {0.5f, -0.5f, 0.5f},
            .Color = {1.0f, 1.0f, 1.0f},
            .TexCoords = {0.0f, 1.0f},
        },
    };

    std::vector<uint32_t> indices{
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Back face
        4, 5, 6,
        6, 7, 4,

        // Top face
        8, 9, 10,
        10, 11, 8,

        // Bottom face
        12, 13, 14,
        14, 15, 12,

        // Left face
        16, 17, 18,
        18, 19, 16,

        // Right face
        20, 21, 22,
        22, 23, 20,
    };

    AxrSubmesh submesh{
        .VerticesCount = static_cast<uint32_t>(vertices.size()),
        .Vertices = vertices.data(),
        .IndicesCount = static_cast<uint32_t>(indices.size()),
        .Indices = indices.data(),
    };

    const AxrMesh mesh{
        .SubmeshCount = 1,
        .Submeshes = &submesh,
    };

    const AxrResult axrResult = model.setData(1, &mesh);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

// ----------------------------------------- //
// Image Engine Assets
// ----------------------------------------- //

bool axrEngineAssetIsImage(const AxrEngineAssetEnum engineAssetEnum) {
    return engineAssetEnum >= AXR_ENGINE_ASSET_IMAGE_START &&
        engineAssetEnum <= AXR_ENGINE_ASSET_IMAGE_END;
}

const char* axrEngineAssetGetImageName(const AxrEngineAssetEnum engineAssetEnum) {
    if (!axrEngineAssetIsImage(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not an image.");
        return "";
    }

    const auto foundEngineAssetIt = EngineAssetImageNames.find(engineAssetEnum);
    if (foundEngineAssetIt == EngineAssetImageNames.end()) {
        axrLogError("Failed to find name for engine asset: {0}.", static_cast<int>(engineAssetEnum));
        return "";
    }

    return foundEngineAssetIt->second;
}

bool axrEngineAssetIsImageNameReserved(const char* name) {
    for (const auto& engineAssetName : EngineAssetImageNames | std::views::values) {
        if (std::strcmp(engineAssetName, name) == 0) {
            return true;
        }
    }

    return false;
}

AxrResult axrEngineAssetCreateImage(
    const std::string& imageName,
    const AxrEngineAssetEnum engineAssetEnum,
    AxrImage& image
) {
    if (!axrEngineAssetIsImage(engineAssetEnum)) {
        axrLogErrorLocation("Engine asset is not an image.");
        return AXR_ERROR;
    }

    switch (engineAssetEnum) {
        case AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE: {
            return axrEngineAssetCreateImage_MissingTexture(imageName, image);
        }
        case AXR_ENGINE_ASSET_IMAGE_UV_TESTER: {
            return axrEngineAssetCreateImage_UvTester(imageName, image);
        }
        case AXR_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown image engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrEngineAssetCreateImage_MissingTexture(const std::string& imageName, AxrImage& image) {
    const AxrImageConfig imageConfig{
        .Name = imageName.c_str(),
        .FilePath = "",
    };
    image = AxrImage(imageConfig);

    const std::vector<stbi_uc> imageData{
        255, 0, 255,
        0, 0, 0,
        0, 0, 0,
        255, 0, 255,
    };
    const AxrResult axrResult = image.setData(2, 2, AXR_IMAGE_COLOR_CHANNELS_RGB, imageData.data());
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }
    return AXR_SUCCESS;
}

AxrResult axrEngineAssetCreateImage_UvTester(const std::string& imageName, AxrImage& image) {
    const std::string& filePath = axrGetEngineAssetsDirectoryPath().append("images/uv-tester.png").generic_string();

    const AxrImageConfig imageConfig{
        .Name = imageName.c_str(),
        .FilePath = filePath.c_str(),
    };
    image = AxrImage(imageConfig);

    return AXR_SUCCESS;
}
