#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform constants {
    mat4 modelMatrix;
} pushConstants;

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
vec2(0.0, -0.5),
vec2(0.5, 0.5),
vec2(-0.5, 0.5)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = vec3(pushConstants.modelMatrix[3][0], pushConstants.modelMatrix[3][1], pushConstants.modelMatrix[3][2]);
}