// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "sceneUtils.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

glm::mat4 axrTransformGetMatrix(const AxrTransformComponent& transformComponent) {
    const glm::mat4 translate = glm::translate(glm::mat4(1.0f), transformComponent.Position);
    const glm::mat4 rotation = glm::toMat4(transformComponent.Orientation);
    const glm::mat4 scale = glm::scale(glm::mat4(1.0f), transformComponent.Scale);
    return translate * rotation * scale;
}
