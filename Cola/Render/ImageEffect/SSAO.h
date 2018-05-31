#pragma once
#include "../Shader.h"
#include "../Mesh.h"
#include <vector>

class SSAO
{
public:
	SSAO(int w, int h);
	~SSAO();
	void Resize(int w, int h);
	void Render();
	void SetParm(const glm::mat4& vp, const glm::vec3& viewPos);
	void SetResultTo(unsigned int slot);
private:
	void Init(int w, int h);
	void Clear();
	unsigned int mFBO;
	unsigned int mResult;
	unsigned int mOffset;
	Shader mSSAOShd;
	Mesh mScreen;
	std::vector<glm::vec3> mSamples;
};