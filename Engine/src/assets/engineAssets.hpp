#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ---------------------------------------------------------------------------------- //
//                               Shader Engine Assets                                 //
// ---------------------------------------------------------------------------------- //

/// Check if the given engine asset is a shader
/// @param engineAssetEnum Engine asset to check
/// @returns True if the given engine asset is a shader
bool axrEngineAssetIsShader(AxrEngineAssetEnum engineAssetEnum);

/// Get the name for the given shader engine asset
/// @param engineAssetEnum Engine asset to get the name of
/// @returns The name of the given shader engine asset
const char* axrEngineAssetGetShaderName(AxrEngineAssetEnum engineAssetEnum);
/// Check if the given name is reserved for a shader engine asset
/// @param name Name to check
/// @returns True if the given name is reserved for a shader engine asset
bool axrEngineAssetIsShaderNameReserved(const char* name);

/// Create an engine asset shader
/// @param engineAssetEnum Shader engine asset
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateShader(AxrEngineAssetEnum engineAssetEnum, AxrShader& shader);

/// Create the 'Default Vert' engine asset shader
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateShader_DefaultVert(AxrShader& shader);
/// Create the 'Default Frag' engine asset shader
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateShader_DefaultFrag(AxrShader& shader);

// ---------------------------------------------------------------------------------- //
//                               Buffer Engine Assets                                 //
// ---------------------------------------------------------------------------------- //

/// Shader buffer scope enum
enum AxrShaderBufferScopeEnum {
    AXR_SHADER_BUFFER_SCOPE_UNDEFINED = 0,
    AXR_SHADER_BUFFER_SCOPE_SCENE,
    AXR_SHADER_BUFFER_SCOPE_MATERIAL,
    AXR_SHADER_BUFFER_SCOPE_MODEL,
};

/// Engine asset buffer properties
struct AxrEngineAssetBufferProperties {
    const char* Name;
    AxrShaderBufferScopeEnum Scope;
};

/// Check if the given engine asset is a uniform buffer
/// @param engineAssetEnum Engine asset to check
/// @returns True if the given engine asset is a uniform buffer
bool axrEngineAssetIsUniformBuffer(AxrEngineAssetEnum engineAssetEnum);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
/// Check if the given engine asset is a push constant buffer
/// @param engineAssetEnum Engine asset to check
/// @returns True if the given engine asset is a push constant buffer
bool axrEngineAssetIsPushConstantBuffer(AxrEngineAssetEnum engineAssetEnum);
#endif

/// Get the scope of the named buffer engine asset
/// @param bufferName Buffer name
/// @returns Buffer scope
AxrShaderBufferScopeEnum axrEngineAssetGetBufferScope(const char* bufferName);

/// Get the name for the given buffer engine asset
/// @param engineAssetEnum Engine asset to get the name of
/// @returns The name of the given buffer engine asset
const char* axrEngineAssetGetBufferName(AxrEngineAssetEnum engineAssetEnum);
/// Get the name for the given uniform buffer engine asset
/// @param engineAssetEnum Engine asset to get the name of
/// @returns The name of the given uniform buffer engine asset
const char* axrEngineAssetGetUniformBufferName(AxrEngineAssetEnum engineAssetEnum);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
/// Get the name for the given push constant buffer engine asset
/// @param engineAssetEnum Engine asset to get the name of
/// @returns The name of the given push constant buffer engine asset
const char* axrEngineAssetGetPushConstantBufferName(AxrEngineAssetEnum engineAssetEnum);
#endif

/// Check if the given name is reserved for a buffer engine asset
/// @param name Name to check
/// @returns True if the given name is reserved for a buffer engine asset
bool axrEngineAssetIsBufferNameReserved(const char* name);
/// Check if the given name is reserved for a uniform buffer engine asset
/// @param name Name to check
/// @returns True if the given name is reserved for a uniform buffer engine asset
bool axrEngineAssetIsUniformBufferNameReserved(const char* name);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
/// Check if the given name is reserved for a push constant buffer engine asset
/// @param name Name to check
/// @returns True if the given name is reserved for a push constant buffer engine asset
bool axrEngineAssetIsPushConstantBufferNameReserved(const char* name);
#endif

// ---------------------------------------------------------------------------------- //
//                              Material Engine Assets                                //
// ---------------------------------------------------------------------------------- //

/// Create the 'Default Material' engine asset material
/// @param materialName Material name
/// @param materialValues Material values
/// @param material Output created material
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateMaterial_DefaultMaterial(
    const std::string& materialName,
    AxrEngineAssetMaterial_DefaultMaterial materialValues,
    AxrMaterial& material
);

// ---------------------------------------------------------------------------------- //
//                                Model Engine Assets                                 //
// ---------------------------------------------------------------------------------- //

/// Check if the given engine asset is a model
/// @param engineAssetEnum Engine asset to check
/// @returns True if the given engine asset is a model
bool axrEngineAssetIsModel(AxrEngineAssetEnum engineAssetEnum);

/// Create an engine asset model
/// @param modelName Model name
/// @param engineAssetEnum Model engine asset
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateModel(const std::string& modelName, AxrEngineAssetEnum engineAssetEnum, AxrModel& model);

/// Create the 'Triangle' engine asset model
/// @param modelName Model name
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateModel_Triangle(const std::string& modelName, AxrModel& model);

/// Create the 'Square' engine asset model
/// @param modelName Model name
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateModel_Square(const std::string& modelName, AxrModel& model);

/// Create the 'Cube' engine asset model
/// @param modelName Model name
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetCreateModel_Cube(const std::string& modelName, AxrModel& model);

// ---------------------------------------------------------------------------------- //
//                                Image Engine Assets                                 //
// ---------------------------------------------------------------------------------- //

/// Check if the given engine asset is an image
/// @param engineAssetEnum Engine asset to check
/// @returns True if the given engine asset is an image
bool axrEngineAssetIsImage(AxrEngineAssetEnum engineAssetEnum);

/// Get the file path to the given image engine asset
/// @param engineAssetEnum Engine asset enum
/// @param imageFilePath Output image file path
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrEngineAssetGetImagePath(AxrEngineAssetEnum engineAssetEnum, std::string& imageFilePath);
