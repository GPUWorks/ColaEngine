#pragma once
#include "../Shader.h"
#include "../Mesh.h"

class FinalPass
{
public:
	FinalPass();
	void Render();
	void DebugView(int n);
private:
	Shader mFinalShd;
	Shader mDepthShd;
	Shader mAlbedoShd;
	Shader mNormalShd;
	Shader mRoughnessShd;
	Shader mMetallicShd;
	Mesh mScreen;
	int mDebugView;
};