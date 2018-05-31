#version 330 core

in vec3 pos;

uniform mat4 vp;

out vec3 fragPos;

void main()
{
    fragPos = pos;
    gl_Position = vp * vec4(pos, 1.0f);
}