#pragma once
#include "../Shader.h"
#include "../Mesh.h"

class Bloom
{
public:
	Bloom(int w, int h);
	~Bloom();
	void Resize(int w, int h);
	void Render();
	void SetResultTo(unsigned int slot);
private:
	void Init(int w, int h);
	void Clear();

	unsigned int mHighLightFBO;
	unsigned int mHighLightTex;

	unsigned int mGaussFBO[2];
	unsigned int mGaussBlur[2];

	unsigned int mFBO;
	unsigned int mResult;

	Shader mHighLightShd;
	Shader mBloomHShd;
	Shader mBloomVShd;
	Shader mBlendShd;

	Mesh mScreen;
};