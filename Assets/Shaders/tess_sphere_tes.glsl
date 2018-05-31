#version 400 core

/*
* 配置图元生成器
* 具体见红宝书p368
*/
layout(quads, equal_spacing, ccw) in;

out t2f
{
    vec3 pos;
    vec3 normal;
} Output;

uniform mat4 model;
uniform mat4 mvp;
uniform float radius;

void main()
{
	float u = gl_TessCoord.x;
	float omu = 1 - u;
	float v = gl_TessCoord.y;
	float omv = 1 - v;
	vec4 pos =
		omu * omv * gl_in[0].gl_Position +
		u * omv * gl_in[1].gl_Position +
		u * v * gl_in[2].gl_Position +
		omu * v * gl_in[3].gl_Position;
	
	Output.normal = normalize(pos.xyz);
	pos = vec4(Output.normal * radius, 1.0f);
	Output.pos = (model * pos).xyz;
	gl_Position = mvp * pos;
}