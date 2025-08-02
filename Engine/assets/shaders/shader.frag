#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform MaterialProperties{
    float alphaCutoff;
};

layout(location = 0) out vec4 outColor;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);
    // Discard transparent pixels
    if (texColor.a < alphaCutoff) {
        discard;
    }

    outColor = vec4(fragColor * texColor.rgb, texColor.a);
}