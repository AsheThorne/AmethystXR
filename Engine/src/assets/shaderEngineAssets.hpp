#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Create an engine asset shader
/// @param engineAssetEnum Shader engine asset
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetShader(AxrShaderEngineAssetEnum engineAssetEnum, AxrShader& shader);

/// Create the 'Default Vert' engine asset shader
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetShader_DefaultVert(AxrShader& shader);
/// Create the 'Default Frag' engine asset shader
/// @param shader Output created shader
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetShader_DefaultFrag(AxrShader& shader);