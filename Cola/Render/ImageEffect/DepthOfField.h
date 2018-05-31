#pragma once
#include "../Shader.h"
#include "../Mesh.h"

class DepthOfField
{
public:
	DepthOfField(int w, int h);
	~DepthOfField();
	void Resize(int w, int h);
	void Render();
	void SetResultTo(unsigned int slot);
private:
	void Init(int w, int h);
	void Clear();

	unsigned int mCoCFBO;
	unsigned int mCoC;

	unsigned int mBlurFBO[2];
	unsigned int mBlur[2];

	Shader mCoCShd;
	Shader mBlurHShd;
	Shader mBlurVShd;

	Mesh mScreen;
};