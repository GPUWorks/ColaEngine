#pragma once
#include "../Shader.h"
#include "../Mesh.h"
#include "../HDRI.h"

class PrePass
{
public:
	PrePass(int resolution);
	~PrePass();
	void Render(const char* hdri);
	void SetSkyTexTo(unsigned int slot);
	void SetDiffEnvTo(unsigned int slot);
	void SetSpecEnvTo(unsigned int slot);
	void SetBrdfLutTo(unsigned int slot);
private:
	Shader mDiffEnvShd;
	Shader mSpecEnvShd;
	Shader mBrdfLutShd;
	Mesh mSkyCube;
	Mesh mScreen;

	HDRI* mEnvHDRI;

	glm::mat4 mViewProj[6];
	int mRes;
	unsigned int mDiffEnvFBO;
	unsigned int mDiffEnvMap;

	unsigned int mSpecEnvFBO;
	unsigned int mSpecEnvMap;

	unsigned int mBrdfLutFBO;
	unsigned int mBrdfLutTex;
};