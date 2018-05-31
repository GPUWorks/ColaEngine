/*****************************
* 细分控制着色器用于计算控制点 *
*****************************/
#version 400 core

// 设置输出控制点数量
layout(vertices = 4) out;

uniform float tessLevel;

void main()
{
	/*
	* gl_in, gl_out为输入和输出顶点数组
	* gl_InvocationID是当前输出控制点的索引
	* 此外，此阶段着色器输入变量还有
	* gl_PrimitiveID 当前输入Patch的图元索引
	* gl_PatchVerticesIn 输入Patch的顶点数量
	* gl_PatchVerticesOut 输出Patch的顶点数量
	* 常见的作法是直接将输入面片顶点输出
	*/
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	/*
	* 设置细分等级
	* gl_TessLevelOuter设置外侧划分方式，包含4个元素
	* gl_TessLevelInner设置内部划分方式，包含2个元素
	* OpenGL支持三种细分域：四边形、三角形和等值集合
	* 对于不同的细分域，需使用的细分等级数量不同
	*/
	gl_TessLevelOuter[0] = tessLevel;
	gl_TessLevelOuter[1] = tessLevel;
	gl_TessLevelOuter[2] = tessLevel;
	gl_TessLevelOuter[3] = tessLevel;
	gl_TessLevelInner[0] = tessLevel;
	gl_TessLevelInner[1] = tessLevel;
}