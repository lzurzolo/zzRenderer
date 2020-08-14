#version 450 core

out vec4 FragColor;

uniform float metallicFactor;
uniform vec4 roughness;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}