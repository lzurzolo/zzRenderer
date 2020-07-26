#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;
layout (location = 3) in vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outPos;
out vec3 outNormal;
out vec2 outTexel;

void main()
{
    mat4 MVP = projection * view * model;
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0f);
    outPos = pos;
    outNormal = normalize(mat3(MVP) * normal);
    outTexel = texel;
}
