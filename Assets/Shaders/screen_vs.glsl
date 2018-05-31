#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 4) in vec2 uv;

out v2f
{
	vec2 uv;
} Output;

void main()
{
	Output.uv = uv;
	gl_Position = vec4(pos, 1.0);
}