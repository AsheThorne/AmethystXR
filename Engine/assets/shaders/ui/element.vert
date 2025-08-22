#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
};

layout (binding = 0) uniform SceneData {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
    float cameraNearPlane;
    float cameraFarPlane;
};

layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}