#include "../Basic/Shape.h"

Mesh::Vertex gScreenVer[] =
{
	{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(0, 0), glm::vec4(), glm::ivec4() },
	{ glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(1, 0), glm::vec4(), glm::ivec4() },
	{ glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(1, 1), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(0, 1), glm::vec4(), glm::ivec4() },
};

unsigned int gScreenInd[] =
{
	0, 1, 2,
	0, 2, 3
};

Mesh::Vertex gCubeVer[] =
{
	// front
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f,  0.5f, 0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	// back
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	// left
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	// right
	{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	// top
	{ glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, 0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	// bottom
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
	{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(), glm::vec3(), glm::vec3(), glm::vec2(), glm::vec4(), glm::ivec4() },
};

unsigned int gSkyCubeInd[] =
{
	0,  3,  2,  0,  2,  1,
	4,  7,  6,  4,  6,  5,
	8,  11, 10,  8, 10, 9,
	12, 15, 14, 12, 14, 13,
	16, 19, 18, 16, 18, 17,
	20, 23, 22, 20, 22, 21
};