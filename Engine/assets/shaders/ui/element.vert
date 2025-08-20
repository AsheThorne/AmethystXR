#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
};

layout(location = 0) out vec2 fragTexCoord;

void main() {
//    gl_Position = modelMatrix * vec4(inPosition, 1.0);
    gl_Position = vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}