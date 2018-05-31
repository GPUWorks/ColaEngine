#include "../Render/Pass/LightingPass.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>

LightingPass::LightingPass(int w, int h) :
	mLightingShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/lighting_fs.glsl"),
	mSkyShd("../Assets/Shaders/sky_vs.glsl", "../Assets/Shaders/sky_fs.glsl"),
	mScreen(gScreenVer, 4, gScreenInd, 6),
	mSkyCube(gCubeVer, 24, gSkyCubeInd, 36)
{
	Init(w, h);
	mLightingShd.Use();
	mLightingShd.SetUniform("PosDepth", 1);
	mLightingShd.SetUniform("NormalAO", 2);
	mLightingShd.SetUniform("AlbedoRoughness", 3);
	mLightingShd.SetUniform("EmissiveMetallic", 4);

	mLightingShd.SetUniform("diffEnvMap", 5);
	mLightingShd.SetUniform("specEnvMap", 6);
	mLightingShd.SetUniform("brdfLutTex", 7);
}

LightingPass::~LightingPass()
{
	Clear();
}

void LightingPass::Resize(int w, int h)
{
	Clear();
	Init(w, h);
}

void LightingPass::SetParm(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos)
{
	mSkyShd.Use();
	glm::mat4 vp4Sky = proj * glm::mat4(glm::mat3(view));
	mSkyShd.SetUniform("vp", vp4Sky);

	mLightingShd.Use();
	mLightingShd.SetUniform("viewPos", viewPos);
}

void LightingPass::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	mSkyShd.Use();
	mSkyCube.Draw();

	mLightingShd.Use();
	mScreen.Draw();

	glBindTexture(GL_TEXTURE_2D, mResult);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void LightingPass::SetResultTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mResult);
}

void LightingPass::Init(int w, int h)
{
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenTextures(1, &mResult);
	glBindTexture(GL_TEXTURE_2D, mResult);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResult, 0);
}

void LightingPass::Clear()
{
	glDeleteFramebuffers(1, &mFBO);
	glDeleteTextures(1, &mResult);
}
