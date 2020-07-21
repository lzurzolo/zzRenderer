#version 450

out vec4 FragColor;

in vec3 outPos;
in vec3 outNormal;
in vec2 outTexel;

uniform sampler2D metallicRoughness;

void main()
{
    FragColor = texture(metallicRoughness, outTexel);
}
