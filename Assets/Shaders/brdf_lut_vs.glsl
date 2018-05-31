#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 4) in vec2 texcoord;

out vec2 uv;

void main()
{
    uv = texcoord;
    gl_Position = vec4(pos, 1.0f);
}