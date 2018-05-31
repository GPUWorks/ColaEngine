#pragma once
#include "../Shader.h"
#include "../Mesh.h"

class LightingPass
{
public:
	LightingPass(int w, int h);
	~LightingPass();
	void Resize(int w, int h);
	void SetParm(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);
	void Render();
	void SetResultTo(unsigned int slot);
private:
	void Init(int w, int h);
	void Clear();
	Shader mLightingShd;
	Shader mSkyShd;
	Mesh mScreen;
	Mesh mSkyCube;
	unsigned int mFBO;
	unsigned int mResult;
};