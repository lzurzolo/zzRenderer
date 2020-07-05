#version 450 core

out vec4 FragColor;

uniform float metallicFactor;
uniform vec4 roughness;

void main()
{
    if(metallicFactor < 0.5)
    {
        FragColor = roughness;
    }
    else
    {
        FragColor = roughness * metallicFactor;
    }
}