// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "modelEngineAssets.hpp"
#include "axr/logger.h"
#include "model.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

AxrResult axrCreateEngineAssetModel(
    const char* modelName,
    const AxrModelEngineAssetEnum engineAssetEnum,
    AxrModel& model
) {
    switch (engineAssetEnum) {
        case AXR_MODEL_ENGINE_ASSET_TRIANGLE: {
            return axrCreateEngineAssetModel_Triangle(modelName, model);
        }
        case AXR_MODEL_ENGINE_ASSET_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown model engine asset.");
            return AXR_ERROR;
        }
    }
}

AxrResult axrCreateEngineAssetModel_Triangle(const char* modelName, AxrModel& model) {
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
        .Name = modelName,
        .FilePath = nullptr,
        .MeshesCount = 1,
        .Meshes = &mesh
    };

    model = AxrModel(modelConfig);

    return AXR_SUCCESS;
}
