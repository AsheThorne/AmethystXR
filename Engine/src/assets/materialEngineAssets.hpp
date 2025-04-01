#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Create the 'Default Material' engine asset material
/// @param materialName Material name
/// @param materialValues Material values
/// @param material Output created material
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetMaterial_DefaultMaterial(
    const char* materialName,
    AxrMaterialEngineAsset_DefaultMaterial materialValues,
    AxrMaterial& material
);
