// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "assets.h"
#include "../memory/allocator.h"
#include "../platform/platform.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// Macros
// ----------------------------------------- //

#define AXR_ENGINE_ASSET_PREFIX "AXR:"
#define AXR_ENGINE_ASSET_SHADER_PREFIX AXR_ENGINE_ASSET_PREFIX "Shader_"

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrAssets::AxrAssets() = default;

AxrAssets::~AxrAssets() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrAssets& AxrAssets::get() {
    static AxrAssets singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr assets. "
AxrResult AxrAssets::setup(const Config& config) {
    assert(!m_IsSetup);

    AxrResult axrResult = setupRegistries();
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to initialize registries.");
        return axrResult;
    }

    axrResult = registerEngineAssets();
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to register engine assets.");
        return axrResult;
    }

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrAssets::shutDown() {
    cleanupRegistries();
    m_IsSetup = false;
}
#define AXR_FUNCTION_FAILED_STRING "Failed to register shader asset. "
AxrResult AxrAssets::registerShaderAsset(const AxrShaderAssetConfig& config) {
    if (m_ShaderRegistry.exists(config.ID)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "A shader with the ID: {0} already exists.", config.ID);
        return AXR_ERROR_DUPLICATE;
    }

    const AxrResult axrResult = isShaderAssetConfigValid(config);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Shader config is invalid.");
        return axrResult;
    }

    const auto assetIterator = m_ShaderRegistry.insert(config.ID, AxrShaderAsset(config));
    if (assetIterator == m_ShaderRegistry.end()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to add shader to registry.");
        return AXR_ERROR_UNKNOWN;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

AxrResult AxrAssets::setupRegistries() {
    m_ShaderRegistry = AxrUnorderedMap_Dynamic<AxrID, AxrShaderAsset>(256, &AxrAllocator::get().EngineDataAllocator);
    if (!m_ShaderRegistry.allocated()) [[unlikely]] {
        axrLogError("Failed to allocate shader registry.");
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    return AXR_SUCCESS;
}

void AxrAssets::cleanupRegistries() {
    m_ShaderRegistry.~AxrUnorderedMap_Dynamic();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to register engine assets. "
AxrResult AxrAssets::registerEngineAssets() {
    AxrResult axrResult = AXR_SUCCESS;

    // Start at 1 since 0 is an undefined asset
    for (int engineAsset = 1; engineAsset < AXR_ENGINE_ASSETS_END; ++engineAsset) {
        if (engineAsset <= AXR_ENGINE_ASSET_SHADERS_END) {
            axrResult = registerShaderAsset(getEngineAssetShaderConfig(static_cast<AxrEngineAssetEnum>(engineAsset)));

            if (AXR_FAILED(axrResult)) [[unlikely]] {
                axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to register shader asset.");
                return axrResult;
            }

            continue;
        }

        axrLogWarning("Failed to register engine asset {}. Unknown asset.", engineAsset);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get engine asset shader config. "
AxrShaderAssetConfig AxrAssets::getEngineAssetShaderConfig(const AxrEngineAssetEnum engineAsset) {
    const char8_t* relativeAssetFile;
    AxrID_T AssetId;

    switch (engineAsset) {
        case AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT: {
            relativeAssetFile = u8"shaders/testShader.vert.spv";
            AssetId = AXR_ENGINE_ASSET_SHADER_PREFIX u8"DefaultVert"_id;
            break;
        }
        case AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG: {
            relativeAssetFile = u8"shaders/testShader.frag.spv";
            AssetId = AXR_ENGINE_ASSET_SHADER_PREFIX u8"DefaultFrag"_id;
            break;
        }
        default: {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Unknown shader engine asset.");
            return {};
        }
    }

    const size_t assetFileLength = std::char_traits<char8_t>::length(relativeAssetFile);

    AxrPath assetPath;
    const AxrResult axrResult = AxrPlatform::get().getEngineAssetsPath(assetFileLength, assetPath);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get engine assets path.");
        return {};
    }

    assetPath.appendPath(relativeAssetFile);

    auto config = AxrShaderAssetConfig{
        .ID = AssetId,
    };

    std::char_traits<char8_t>::copy(config.FilePath, assetPath.data(), AXR_MAX_FILE_PATH_SIZE);

    return config;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrResult AxrAssets::isShaderAssetConfigValid(const AxrShaderAssetConfig& config) {
    const bool pathExists = AxrPlatform::get().pathExists(config.FilePath);
    if (!pathExists) [[unlikely]] {
        return AXR_ERROR_INVALID_PATH;
    }

    return AXR_SUCCESS;
}
