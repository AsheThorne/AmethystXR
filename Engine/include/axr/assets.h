#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "common/defines.h"
#include "common/types.h"

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Axr Engine Asset Enum
enum AxrEngineAssetEnum {
    AXR_ENGINE_ASSET_UNDEFINED = 0,

    // ---- Shaders ----
    AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT,
    AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG,

    AXR_ENGINE_ASSET_SHADERS_BEGIN = AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT,
    AXR_ENGINE_ASSET_SHADERS_END = AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG,

    // ---- End of engine assets ----
    AXR_ENGINE_ASSETS_END,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Shader asset config
struct AxrShaderAssetConfig {
    AxrID_T ID;
    char8_t FilePath[AXR_MAX_FILE_PATH_SIZE];
};

/// Scene asset config
struct AxrSceneAssetConfig {
    AxrID_T ID;
    char8_t FilePath[AXR_MAX_FILE_PATH_SIZE];
};
