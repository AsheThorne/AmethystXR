#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(std140, binding = 1) uniform UIElement {
    vec2 position;
    vec2 size;
    vec4 backgroundColor;
};

layout(location = 0) out vec4 outColor;

void main() {
    outColor = backgroundColor;
}