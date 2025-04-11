#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/graphicsSystem.h"

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>

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

// ---- Vertex Attributes ----

/// Shader vertex attribute enum
enum AxrShaderVertexAttributeEnum {
    AXR_SHADER_VERTEX_ATTRIBUTE_UNKNOWN = 0,
    AXR_SHADER_VERTEX_ATTRIBUTE_POSITION,
    AXR_SHADER_VERTEX_ATTRIBUTE_COLOR,
    AXR_SHADER_VERTEX_ATTRIBUTE_TEX_COORDS,
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

// ---- Shader Vertex Attribute ----

/// Shader Vertex Attribute
struct AxrShaderVertexAttribute {
    AxrShaderVertexAttributeEnum Type;
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
    uint32_t VertexAttributesCount;
    AxrShaderVertexAttribute* VertexAttributes;
    uint32_t BufferLayoutsCount;
    AxrShaderBufferLayout_T* BufferLayouts;
};

/// AxrVertexShaderProperties Handle Type
typedef AxrVertexShaderProperties* AxrVertexShaderProperties_T;
/// const AxrVertexShaderProperties Handle Type
typedef const AxrVertexShaderProperties* AxrVertexShaderPropertiesConst_T;

/// Fragment shader properties
struct AxrFragmentShaderProperties {
    const AxrShaderStageEnum Type = AXR_SHADER_STAGE_FRAGMENT;
    uint32_t BufferLayoutsCount;
    AxrShaderBufferLayout_T* BufferLayouts;
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

    // ---- Shader Vertex Attributes ----

    /// Clone the given shader vertex attribute
    /// @param vertexAttribute Shader vertex attribute to clone
    /// @returns The cloned shader vertex attribute
    AXR_API AxrShaderVertexAttribute axrShaderVertexAttributeClone(AxrShaderVertexAttribute vertexAttribute);
    /// Destroy the given shader vertex attribute
    /// @param vertexAttribute Shader vertex attribute to destroy
    AXR_API void axrShaderVertexAttributeDestroy(AxrShaderVertexAttribute* vertexAttribute);

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
    /// Check if the given shader config is valid
    /// @param shaderConfig Shader config to check
    /// @returns True if the given shader config is valid
    AXR_API bool axrShaderConfigIsValid(const AxrShaderConfig* shaderConfig);
    /// Get the shader's name
    /// @param shader Shader to use
    /// @returns The shader's name
    AXR_API const char* axrShaderGetName(AxrShader_T shader);
}

// ---------------------------------------------------------------------------------- //
//                                   Shader Values                                    //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Shader buffer link type enum
enum AxrShaderBufferLinkEnum {
    AXR_SHADER_BUFFER_LINK_UNDEFINED = 0,
    AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER,
    AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER,
    AXR_SHADER_BUFFER_LINK_PUSH_CONSTANTS_BUFFER,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Shader Buffer Link Base Structure
struct AxrShaderBufferLinkStructure {
    AxrShaderBufferLinkEnum Type = AXR_SHADER_BUFFER_LINK_UNDEFINED;
};

/// AxrShaderBufferLinkStructure Handle Type
typedef AxrShaderBufferLinkStructure* AxrShaderBufferLink_T;
/// Const AxrShaderBufferLinkStructure Handle Type
typedef const AxrShaderBufferLinkStructure* AxrShaderBufferLinkConst_T;

/// Shader Uniform Buffer Link
struct AxrShaderUniformBufferLink {
    const AxrShaderBufferLinkEnum Type = AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER;
    uint32_t Binding;
    const char* BufferName;
};

/// AxrShaderUniformBufferLink Handle Type
typedef AxrShaderUniformBufferLink* AxrShaderUniformBufferLink_T;
/// Const AxrShaderUniformBufferLink Handle Type
typedef const AxrShaderUniformBufferLink* AxrShaderUniformBufferLinkConst_T;

/// Shader Image Sampler Buffer Link
struct AxrShaderImageSamplerBufferLink {
    const AxrShaderBufferLinkEnum Type = AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER;
    uint32_t Binding;
    const char* ImageName;
};

/// AxrShaderImageSamplerBufferLink Handle Type
typedef AxrShaderImageSamplerBufferLink* AxrShaderImageSamplerBufferLink_T;
/// Const AxrShaderImageSamplerBufferLink Handle Type
typedef const AxrShaderImageSamplerBufferLink* AxrShaderImageSamplerBufferLinkConst_T;

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
/// Shader Push Constants Buffer Link
struct AxrShaderPushConstantsBufferLink {
    const AxrShaderBufferLinkEnum Type = AXR_SHADER_BUFFER_LINK_PUSH_CONSTANTS_BUFFER;
    const char* BufferName;
};

/// AxrShaderPushConstantsBufferLink Handle Type
typedef AxrShaderPushConstantsBufferLink* AxrShaderPushConstantsBufferLink_T;
/// Const AxrShaderPushConstantsBufferLink Handle Type
typedef const AxrShaderPushConstantsBufferLink* AxrShaderPushConstantsBufferLinkConst_T;
#endif

/// Shader Values
struct AxrShaderValues {
    uint32_t BufferLinksCount;
    AxrShaderBufferLink_T* BufferLinks;
};

/// AxrShaderValues Handle Type
typedef AxrShaderValues* AxrShaderValues_T;
/// Const AxrShaderValues Handle Type
typedef const AxrShaderValues* AxrShaderValuesConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    // ---- Shader Buffer Links ----

    /// Clone the given shader buffer link
    /// @param bufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    AXR_API AxrShaderBufferLink_T axrShaderBufferLinkClone(AxrShaderBufferLinkConst_T bufferLink);
    /// Destroy the given shader buffer link
    /// @param bufferLink Shader buffer link to destroy
    AXR_API void axrShaderBufferLinkDestroy(AxrShaderBufferLink_T* bufferLink);

    /// Clone the given shader uniform buffer link
    /// @param bufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    AXR_API AxrShaderUniformBufferLink_T axrShaderUniformBufferLinkClone(
        AxrShaderUniformBufferLinkConst_T bufferLink
    );
    /// Destroy the given shader uniform buffer link
    /// @param bufferLink Shader buffer link to destroy
    AXR_API void axrShaderUniformBufferLinkDestroy(AxrShaderUniformBufferLink_T* bufferLink);

    /// Clone the given shader image sampler buffer link
    /// @param bufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    AXR_API AxrShaderImageSamplerBufferLink_T axrShaderImageSamplerBufferLinkClone(
        AxrShaderImageSamplerBufferLinkConst_T bufferLink
    );
    /// Destroy the given shader image sampler buffer link
    /// @param bufferLink Shader buffer link to destroy
    AXR_API void axrShaderImageSamplerBufferLinkDestroy(AxrShaderImageSamplerBufferLink_T* bufferLink);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Clone the given shader push constants buffer link
    /// @param bufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    AXR_API AxrShaderPushConstantsBufferLink_T axrShaderPushConstantsBufferLinkClone(
        AxrShaderPushConstantsBufferLinkConst_T bufferLink
    );
    /// Destroy the given shader push constants buffer link
    /// @param bufferLink Shader buffer link to destroy
    AXR_API void axrShaderPushConstantsBufferLinkDestroy(AxrShaderPushConstantsBufferLink_T* bufferLink);
#endif

    // ---- Shader Values ----

    /// Check if the given shader values are valid
    /// @param values Shader values to check
    /// @returns True if the given shader values are valid
    AXR_API bool axrShaderValuesIsValid(AxrShaderValuesConst_T values);
    /// Clone the given shader values
    /// @param values Shader values to clone
    /// @returns The cloned shader values
    AXR_API AxrShaderValues_T axrShaderValuesClone(AxrShaderValuesConst_T values);
    /// Destroy the given shader values
    /// @param values Shader values to destroy
    AXR_API void axrShaderValuesDestroy(AxrShaderValues_T* values);
}

// ---------------------------------------------------------------------------------- //
//                                  Material Assets                                   //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Material Config
struct AxrMaterialConfig {
    const char* Name;
    const char* VertexShaderName;
    const char* FragmentShaderName;
    AxrShaderValues_T VertexShaderValues;
    AxrShaderValues_T FragmentShaderValues;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrMaterial Handle
typedef class AxrMaterial* AxrMaterial_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Check if the given material config is valid
    /// @param materialConfig Material config to check
    /// @returns True if the given material config is valid
    AXR_API bool axrMaterialConfigIsValid(const AxrMaterialConfig* materialConfig);
    /// Get the material's name
    /// @param material Material to use
    /// @returns The material's name
    AXR_API const char* axrMaterialGetName(AxrMaterial_T material);
}

// ---------------------------------------------------------------------------------- //
//                                   Model Assets                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Vertex
struct AxrVertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords;
};

/// Mesh
struct AxrMesh {
    uint32_t VerticesCount;
    AxrVertex* Vertices;
    uint32_t IndicesCount;
    uint32_t* Indices;
};

/// Model Config
struct AxrModelConfig {
    const char* Name;
    const char* FilePath;
    uint32_t MeshesCount;
    AxrMesh* Meshes;
};

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrModel Handle
typedef class AxrModel* AxrModel_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Clone the given vertices
    /// @param verticesCount Number of vertices in the given array
    /// @param vertices Vertex array to clone
    /// @returns A cloned array of the given vertices
    AXR_API AxrVertex* axrMeshCloneVertices(uint32_t verticesCount, const AxrVertex* vertices);
    /// Clone the given indices
    /// @param indicesCount Number of indices in the given array
    /// @param indices Index array to clone
    /// @returns A cloned array of the given indices
    AXR_API uint32_t* axrMeshCloneIndices(uint32_t indicesCount, const uint32_t* indices);
    /// Clone the given meshes
    /// @param meshesCount Number of meshes in the given array
    /// @param meshes Mesh array to clone
    /// @returns A cloned array of the given meshes
    AXR_API AxrMesh* axrModelCloneMeshes(uint32_t meshesCount, const AxrMesh* meshes);

    /// Get the model's name
    /// @param model Model to use
    /// @returns The model's name
    AXR_API const char* axrModelGetName(AxrModel_T model);
}

// ---------------------------------------------------------------------------------- //
//                               Engine Defined Assets                                //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

// ---- Shaders ----

/// Axr engine defined shader enum
enum AxrShaderEngineAssetEnum {
    AXR_SHADER_ENGINE_ASSET_UNDEFINED = 0,
    AXR_SHADER_ENGINE_ASSET_DEFAULT_VERT,
    AXR_SHADER_ENGINE_ASSET_DEFAULT_FRAG,
};

// ---- Buffers ----

/// Axr engine defined uniform buffer enum
enum AxrUniformBufferEngineAssetEnum {
    AXR_UNIFORM_BUFFER_ENGINE_ASSET_UNDEFINED = 0,
    AXR_UNIFORM_BUFFER_ENGINE_ASSET_VIEW_PROJ_MATRICES,
};

/// Axr engine defined push constants buffer enum
enum AxrPushConstantsBufferEngineAssetEnum {
    AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_UNDEFINED = 0,
    AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX,
};

// ---- Models ----

/// Axr engine defined model enum
enum AxrModelEngineAssetEnum {
    AXR_MODEL_ENGINE_ASSET_UNDEFINED = 0,
    AXR_MODEL_ENGINE_ASSET_TRIANGLE,
};


// ----------------------------------------- //
// Structs
// ----------------------------------------- //

// ---- Materials ----

/// Engine asset material named 'Default Material' values
struct AxrMaterialEngineAsset_DefaultMaterial {
    const char* ImageName;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    // ---- Shaders ----

    /// Check if the given name is reserved as a shader engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a shader engine asset name
    AXR_API bool axrIsShaderNameReserved(const char* name);
    /// Get the name for the given shader engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    AXR_API const char* axrGetShaderEngineAssetName(AxrShaderEngineAssetEnum engineAssetEnum);

    // ---- Buffers ----

    /// Check if the given name is reserved as a uniform buffer engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a uniform buffer engine asset name
    AXR_API bool axrIsUniformBufferNameReserved(const char* name);
    /// Get the name for the given uniform buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    AXR_API const char* axrGetUniformBufferEngineAssetName(AxrUniformBufferEngineAssetEnum engineAssetEnum);

    /// Check if the given name is reserved as a push constants buffer engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a push constants buffer engine asset name
    AXR_API bool axrIsPushConstantsBufferNameReserved(const char* name);
    /// Get the name for the given push constants buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    AXR_API const char* axrGetPushConstantsBufferEngineAssetName(AxrPushConstantsBufferEngineAssetEnum engineAssetEnum);
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
    // ---- All Assets ----

    /// Check if all the assets in the collection have been loaded
    /// @param assetCollection Asset collection to use
    /// @returns True if all assets in the collection have been loaded
    AXR_API bool axrAssetCollectionIsLoaded(AxrAssetCollection_T assetCollection);
    /// Load all assets in the collection
    /// @param assetCollection Asset collection to use
    /// @param graphicsApi Graphics api to use with these assets
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionLoadAssets(
        AxrAssetCollection_T assetCollection,
        AxrGraphicsApiEnum graphicsApi
    );
    /// Unload all assets in the collection
    /// @param assetCollection Asset collection to use
    AXR_API void axrAssetCollectionUnloadAssets(AxrAssetCollection_T assetCollection);

    // ---- Shader ----

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

    // ---- Material ----

    /// Create a new material
    /// @param assetCollection Asset collection to use
    /// @param materialConfig Material config
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateMaterial(
        AxrAssetCollection_T assetCollection,
        const AxrMaterialConfig* materialConfig
    );
    /// Create a material using the engine defined material named 'Default Material'
    /// @param assetCollection Asset collection to use
    /// @param materialName Material name
    /// @param materialValues Material values
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
        AxrAssetCollection_T assetCollection,
        const char* materialName,
        AxrMaterialEngineAsset_DefaultMaterial materialValues
    );

    // ---- Model ----

    /// Create a new model
    /// @param assetCollection Asset collection to use
    /// @param modelConfig Model config
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateModel(
        AxrAssetCollection_T assetCollection,
        const AxrModelConfig* modelConfig
    );

    /// Create a new engine asset model
    /// @param assetCollection Asset collection to use
    /// @param modelName Model name
    /// @param engineAssetEnum Model engine asset
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrAssetCollectionCreateEngineAssetModel(
        AxrAssetCollection_T assetCollection,
        const char* modelName,
        AxrModelEngineAssetEnum engineAssetEnum
    );
}
