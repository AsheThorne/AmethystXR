#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Create an engine asset model
/// @param modelName Model name
/// @param engineAssetEnum Model engine asset
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetModel(const char* modelName, AxrModelEngineAssetEnum engineAssetEnum, AxrModel& model);

/// Create the 'Triangle' engine asset model
/// @param modelName Model name
/// @param model Output created model
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrCreateEngineAssetModel_Triangle(const char* modelName, AxrModel& model);
