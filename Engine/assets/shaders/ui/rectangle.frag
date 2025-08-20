#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(binding = 0) uniform DynamicUniformBuffer {
    vec4 backgroundColor;
};

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(0.0, 0.0, 0.0, 1.0);
}