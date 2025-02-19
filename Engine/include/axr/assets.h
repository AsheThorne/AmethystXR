#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"

// ---------------------------------------------------------------------------------- //
//                                  Shader Properties                                 //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

// ---- Shader Properties ----

/// Shader stage enum
enum AxrShaderStageEnum {
    AXR_SHADER_STAGE_UNDEFINED = 0,
    AXR_SHADER_STAGE_VERTEX,
    AXR_SHADER_STAGE_FRAGMENT,
};

// ---- Shader Buffer Layout ----

/// Shader buffer layout enum
enum AxrShaderBufferLayoutEnum {
    AXR_SHADER_BUFFER_LAYOUT_UNDEFINED = 0,
    AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER,
    AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER,
    AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER,
};

// ---- Vertex Property ----

/// Shader vertex property enum
enum AxrShaderVertexPropertyEnum {
    AXR_SHADER_VERTEX_PROPERTY_UNKNOWN = 0,
    AXR_SHADER_VERTEX_PROPERTY_POSITION,
    AXR_SHADER_VERTEX_PROPERTY_COLOR,
    AXR_SHADER_VERTEX_PROPERTY_TEX_COORDS,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

// ---- Shader Buffer Layout ----

/// Shader Buffer Layout Base Structure
struct AxrShaderBufferLayoutStructure {
    AxrShaderBufferLayoutEnum Type = AXR_SHADER_BUFFER_LAYOUT_UNDEFINED;
};

/// AxrShaderBufferLayoutStructure Handle Type
typedef AxrShaderBufferLayoutStructure* AxrShaderBufferLayout_T;
/// Const AxrShaderBufferLayoutStructure Handle Type
typedef const AxrShaderBufferLayoutStructure* AxrShaderBufferLayoutConst_T;

/// Shader Uniform Buffer Layout
struct AxrShaderUniformBufferLayout {
    const AxrShaderBufferLayoutEnum Type = AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER;
    uint32_t Binding;
    uint64_t BufferSize;
};

/// AxrShaderUniformBufferLayout Handle Type
typedef AxrShaderUniformBufferLayout* AxrShaderUniformBufferLayout_T;
/// Const AxrShaderUniformBufferLayout Handle Type
typedef const AxrShaderUniformBufferLayout* AxrShaderUniformBufferLayoutConst_T;

/// Shader Image Sampler Buffer Layout
struct AxrShaderImageSamplerBufferLayout {
    const AxrShaderBufferLayoutEnum Type = AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER;
    uint32_t Binding;
};

/// AxrShaderImageSamplerBufferLayout Handle Type
typedef AxrShaderImageSamplerBufferLayout* AxrShaderImageSamplerBufferLayout_T;
/// Const AxrShaderImageSamplerBufferLayout Handle Type
typedef const AxrShaderImageSamplerBufferLayout* AxrShaderImageSamplerBufferLayoutConst_T;

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
/// Shader Push Constants Buffer Layout
struct AxrShaderPushConstantsBufferLayout {
    const AxrShaderBufferLayoutEnum Type = AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER;
    uint32_t BufferSize;
};

/// AxrShaderPushConstantsBufferLayout Handle Type
typedef AxrShaderPushConstantsBufferLayout* AxrShaderPushConstantsBufferLayout_T;
/// Const AxrShaderPushConstantsBufferLayout Handle Type
typedef const AxrShaderPushConstantsBufferLayout* AxrShaderPushConstantsBufferLayoutConst_T;
#endif

// ---- Shader Vertex Property ----

/// Shader Vertex Property
struct AxrShaderVertexProperty {
    AxrShaderVertexPropertyEnum Type;
    uint32_t Binding;
    uint32_t Location;
};

// ---- Shader Properties ----

/// Shader Properties Base Structure
struct AxrShaderPropertiesStructure {
    AxrShaderStageEnum Type = AXR_SHADER_STAGE_UNDEFINED;
};

/// AxrShaderPropertiesStructure Handle Type
typedef AxrShaderPropertiesStructure* AxrShaderProperties_T;
/// Const AxrShaderPropertiesStructure Handle Type
typedef const AxrShaderPropertiesStructure* AxrShaderPropertiesConst_T;

/// Vertex shader properties
struct AxrVertexShaderProperties {
    const AxrShaderStageEnum Type = AXR_SHADER_STAGE_VERTEX;
    AxrShaderVertexProperty* VertexProperties;
    uint32_t VertexPropertiesCount;
    AxrShaderBufferLayout_T* BufferLayouts;
    uint32_t BufferLayoutsCount;
};

/// AxrVertexShaderProperties Handle Type
typedef AxrVertexShaderProperties* AxrVertexShaderProperties_T;
/// const AxrVertexShaderProperties Handle Type
typedef const AxrVertexShaderProperties* AxrVertexShaderPropertiesConst_T;

/// Fragment shader properties
struct AxrFragmentShaderProperties {
    const AxrShaderStageEnum Type = AXR_SHADER_STAGE_FRAGMENT;
    AxrShaderBufferLayout_T* BufferLayouts;
    uint32_t BufferLayoutsCount;
};

/// AxrFragmentShaderProperties Handle Type
typedef AxrFragmentShaderProperties* AxrFragmentShaderProperties_T;
/// Const AxrFragmentShaderProperties Handle Type
typedef const AxrFragmentShaderProperties* AxrFragmentShaderPropertiesConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    // ---- Shader Buffer Layouts ----

    /// Clone the given shader buffer layout
    /// @param bufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    AXR_API AxrShaderBufferLayout_T axrShaderBufferLayoutClone(AxrShaderBufferLayoutConst_T bufferLayout);
    /// Destroy the given shader buffer layout
    /// @param bufferLayout Shader buffer layout to destroy
    AXR_API void axrShaderBufferLayoutDestroy(AxrShaderBufferLayout_T* bufferLayout);

    /// Clone the given shader uniform buffer layout
    /// @param bufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    AXR_API AxrShaderUniformBufferLayout_T axrShaderUniformBufferLayoutClone(
        AxrShaderUniformBufferLayoutConst_T bufferLayout
    );
    /// Destroy the given shader uniform buffer layout
    /// @param bufferLayout Shader buffer layout to destroy
    AXR_API void axrShaderUniformBufferLayoutDestroy(AxrShaderUniformBufferLayout_T* bufferLayout);

    /// Clone the given shader image sampler buffer layout
    /// @param bufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    AXR_API AxrShaderImageSamplerBufferLayout_T axrShaderImageSamplerBufferLayoutClone(
        AxrShaderImageSamplerBufferLayoutConst_T bufferLayout
    );
    /// Destroy the given shader image sampler buffer layout
    /// @param bufferLayout Shader buffer layout to destroy
    AXR_API void axrShaderImageSamplerBufferLayoutDestroy(AxrShaderImageSamplerBufferLayout_T* bufferLayout);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Clone the given shader push constants buffer layout
    /// @param bufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    AXR_API AxrShaderPushConstantsBufferLayout_T axrShaderPushConstantsBufferLayoutClone(
        AxrShaderPushConstantsBufferLayoutConst_T bufferLayout
    );
    /// Destroy the given shader push constants buffer layout
    /// @param bufferLayout Shader buffer layout to destroy
    AXR_API void axrShaderPushConstantsBufferLayoutDestroy(AxrShaderPushConstantsBufferLayout_T* bufferLayout);
#endif

    // ---- Shader Vertex Properties ----

    /// Clone the given shader vertex property
    /// @param vertexProperty Shader vertex property to clone
    /// @returns The cloned shader vertex property
    AXR_API AxrShaderVertexProperty axrShaderVertexPropertyClone(AxrShaderVertexProperty vertexProperty);
    /// Destroy the given shader vertex property
    /// @param vertexProperty Shader vertex property to destroy
    AXR_API void axrShaderVertexPropertyDestroy(AxrShaderVertexProperty* vertexProperty);

    // ---- Shader Properties ----

    /// Check if the given shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the given shader properties are valid
    AXR_API bool axrShaderPropertiesIsValid(AxrShaderPropertiesConst_T properties);
    /// Clone the given shader properties
    /// @param properties Shader properties to clone
    /// @returns The cloned shader properties
    AXR_API AxrShaderProperties_T axrShaderPropertiesClone(AxrShaderPropertiesConst_T properties);
    /// Destroy the given shader properties
    /// @param properties Shader properties to destroy
    AXR_API void axrShaderPropertiesDestroy(AxrShaderProperties_T* properties);

    /// Check if the given vertex shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the given shader properties are valid
    AXR_API bool axrVertexShaderPropertiesIsValid(AxrVertexShaderPropertiesConst_T properties);
    /// Clone the given vertex shader properties
    /// @param properties Shader properties to clone
    /// @returns The cloned shader properties
    AXR_API AxrVertexShaderProperties_T axrVertexShaderPropertiesClone(
        AxrVertexShaderPropertiesConst_T properties
    );
    /// Destroy the given vertex shader properties
    /// @param properties Shader properties to destroy
    AXR_API void axrVertexShaderPropertiesDestroy(AxrVertexShaderProperties_T* properties);

    /// Check if the given fragment shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the given shader properties are valid
    AXR_API bool axrFragmentShaderPropertiesIsValid(AxrFragmentShaderPropertiesConst_T properties);
    /// Clone the given fragment shader properties
    /// @param properties Shader properties to clone
    /// @returns The cloned shader properties
    AXR_API AxrFragmentShaderProperties_T axrFragmentShaderPropertiesClone(
        AxrFragmentShaderPropertiesConst_T properties
    );
    /// Destroy the given fragment shader properties
    /// @param properties Shader properties to destroy
    AXR_API void axrFragmentShaderPropertiesDestroy(AxrFragmentShaderProperties_T* properties);
}

// ---------------------------------------------------------------------------------- //
//                                   Shader Assets                                    //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Shader Config
struct AxrShaderConfig {
    const char* Name;
    const char* FilePath;
    AxrShaderProperties_T Properties;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrShader Handle
typedef class AxrShader* AxrShader_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Get the shader's name
    /// @param shader Shader to use
    /// @returns The shader's name
    AXR_API const char* axrShaderGetName(AxrShader_T shader);
    /// Get the shader's file path
    /// @param shader Shader to use
    /// @returns The shader's file path
    AXR_API const char* axrShaderGetFilePath(AxrShader_T shader);
}

// ---------------------------------------------------------------------------------- //
//                               Engine Defined Assets                                //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Axr Engine defined shader enums
enum AxrShaderEngineAssetEnum {
    AXR_SHADER_ENGINE_ASSET_UNDEFINED = 0,
    AXR_SHADER_ENGINE_ASSET_DEFAULT_VERT,
    AXR_SHADER_ENGINE_ASSET_DEFAULT_FRAG,
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Check if the given name is reserved as a shader engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a shader engine asset name
    AXR_API bool axrIsShaderNameReserved(const char* name);
    /// Get the name for the given shader engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    AXR_API const char* axrGetShaderEngineAssetName(AxrShaderEngineAssetEnum engineAssetEnum);
}

// ---------------------------------------------------------------------------------- //
//                                  Asset Collection                                  //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrAssetCollection Handle
typedef class AxrAssetCollection* AxrAssetCollection_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Create a new shader
    /// @param assetCollection Asset collection to use
    /// @param shaderConfig Shader config to use
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateShader(
        AxrAssetCollection_T assetCollection,
        const AxrShaderConfig* shaderConfig
    );
    /// Create a new engine asset shader
    /// @param assetCollection Asset collection to use
    /// @param engineAssetEnum Shader engine asset
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateEngineAssetShader(
        AxrAssetCollection_T assetCollection,
        AxrShaderEngineAssetEnum engineAssetEnum
    );
}
