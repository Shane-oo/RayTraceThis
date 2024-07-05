#version 450

layout (location = 0) in vec3 fragColour;
layout (location = 1) in vec2 fragTextureCoord;

layout (binding = 1) uniform sampler2D texureSampler;

// no built-in variable to output a color, therefore just have to specify own variable where the layout location is 0
layout (location = 0) out vec4 outColour;

void main() {
    outColour = vec4(fragColour * texture(texureSampler, fragTextureCoord).rgb, 1.0);
}