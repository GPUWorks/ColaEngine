#include "../Render/Pass/FinalPass.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>

FinalPass::FinalPass() :
	mFinalShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/final_fs.glsl"),
	mDepthShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/debug/depth_fs.glsl"),
	mAlbedoShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/debug/albedo_fs.glsl"),
	mNormalShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/debug/normal_fs.glsl"),
	mRoughnessShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/debug/roughness_fs.glsl"),
	mMetallicShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/debug/metallic_fs.glsl"),
	mScreen(gScreenVer, 4, gScreenInd, 6),
	mDebugView(-1)
{
	mDepthShd.Use();
	mDepthShd.SetUniform("depth", 1);
	mAlbedoShd.Use();
	mAlbedoShd.SetUniform("albedo", 3);
	mNormalShd.Use();
	mNormalShd.SetUniform("normal", 2);
	mRoughnessShd.Use();
	mRoughnessShd.SetUniform("roughness", 3);
	mMetallicShd.Use();
	mMetallicShd.SetUniform("metallic", 4);
}

void FinalPass::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (mDebugView)
	{
	default: mFinalShd.Use(); break;
	case 0: mDepthShd.Use(); break;
	case 1: mAlbedoShd.Use(); break;
	case 2: mNormalShd.Use(); break;
	case 3: mRoughnessShd.Use(); break;
	case 4: mMetallicShd.Use(); break;
	}
	
	mScreen.Draw();
}

void FinalPass::DebugView(int n)
{
	mDebugView = n;
}
