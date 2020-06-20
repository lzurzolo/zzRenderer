#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(pos.x, pos.y, pos.z, 1.0f);
}
