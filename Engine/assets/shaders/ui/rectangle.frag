#version 450

layout (location = 0) in vec2 fragTexCoord;

layout (std140, binding = 2) uniform UIElement {
    vec2 position;
    vec2 size;
    vec4 backgroundColor;
} uiElement;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = uiElement.backgroundColor;
}