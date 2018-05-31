#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3	pos;
		glm::vec3	normal;
		glm::vec3	tangent;
		glm::vec3	bitangent;
		glm::vec2	uv;
		glm::vec4	weights;
		glm::ivec4	boneIDs;
	};
	Mesh(
		const void* vertices,
		unsigned int numVertices,
		unsigned int* indices = nullptr,
		unsigned int numIndices = 0
	);
	~Mesh();
	void Draw();
	void DrawPatches4();
private:
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
	unsigned int mNumVertices;
	unsigned int mNumIndices;
};