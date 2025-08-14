#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "scene.hpp"

/// Get the matrix for the given transform component
/// @param transformComponent Transform component
/// @returns The transform component matrix
[[nodiscard]] glm::mat4 axrTransformGetMatrix(const AxrTransformComponent& transformComponent);

/// Convert the given clay error type to a string
/// @param errorType Clay error type
/// @returns String equivalent
[[nodiscard]] const char* axrToString(Clay_ErrorType errorType);