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
    // TODO: Add togglable billboard effect 
    // TODO: Add togglable clipping plane offset 
    // Offset it just in front of the camera's near clipping plane
    vec4 position = vec4(inPosition.x, inPosition.y, inPosition.z - (cameraNearPlane * 2 + 0.000001), 1.0);
    gl_Position = viewProjectionMatrix * modelMatrix * position;
    fragTexCoord = inTexCoord;
}