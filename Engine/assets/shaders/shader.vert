#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform constants {
    mat4 modelMatrix;
} pushConstants;

layout (binding = 0) uniform sceneData {
    mat4 viewMatrix;
    mat4 projectionMatrix; 
} scene;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = scene.projectionMatrix * scene.viewMatrix * pushConstants.modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}