#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
};

layout(std140, binding = 0) uniform SceneData {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
    float cameraNearPlane;
    float cameraFarPlane;
};

layout(std140, binding = 1) uniform UIElement {
    vec2 position;
    vec2 size;
} uiElement;

layout(location = 0) out vec2 fragTexCoord;

void main() {
    vec2 test = uiElement.position;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}