#include "../Render/Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const void* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices) :
	mNumVertices(numVertices),
	mNumIndices(numIndices)
{
	unsigned int stride = sizeof(Vertex);
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * stride, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(0);// layout (location = 0) in vec3 pos;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);// layout (location = 1) in vec3 normal;
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);// layout (location = 2) in vec3 tangent;
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);// layout (location = 3) in vec3 bitangent;
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);// layout (location = 4) in vec2 uv;
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*)(14 * sizeof(float)));
	glEnableVertexAttribArray(5);// layout (location = 5) in vec4 weights;
	glVertexAttribIPointer(6, 4, GL_INT, stride, (void*)(18 * sizeof(float)));
	glEnableVertexAttribArray(6);// layout (location = 6) in ivec4 boneIDs;

	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void Mesh::Draw()
{
	glBindVertexArray(mVAO);
	if (mNumIndices) {
		glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
	}
}

void Mesh::DrawPatches4()
{
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glBindVertexArray(mVAO);
	if (mNumIndices) {
		glDrawElements(GL_PATCHES, mNumIndices, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawArrays(GL_PATCHES, 0, mNumVertices);
	}
}
