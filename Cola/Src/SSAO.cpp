#include "../Render/ImageEffect/SSAO.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>
#include <random>

SSAO::SSAO(int w, int h) :
	mSSAOShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/ssao_fs.glsl"),
	mScreen(gScreenVer, 4, gScreenInd, 6)
{
	Init(w, h);
	mSSAOShd.Use();
	mSSAOShd.SetUniform("PosDepth", 1);
	mSSAOShd.SetUniform("NormalAO", 2);
	mSSAOShd.SetUniform("offset", 3);
	mSSAOShd.SetUniform("radius", 0.2f);
	// 生成采样点
	std::uniform_real_distribution<float> random0to1(0.0, 1.0);
	std::default_random_engine e;
	for (int i(0); i < 64; ++i) {
		glm::vec3 sample(
			random0to1(e) * 2.0 - 1.0,
			random0to1(e) * 2.0 - 1.0,
			random0to1(e)
		);
		sample = glm::normalize(sample);
		sample *= random0to1(e);
		float scale = static_cast<float>(i) / 64.0f;
		scale = glm::mix(0.1f, 1.0f, scale * scale);
		sample *= scale;
		mSamples.push_back(sample);
	}
	mSSAOShd.SetUniformv("samples", static_cast<unsigned int>(mSamples.size()), mSamples.data());
	// 生成采样偏移
	std::vector<glm::vec3> sampleOffset;
	for (int i(0); i < 16; ++i)
	{
		glm::vec3 noise(
			random0to1(e) * 2.0 - 1.0,
			random0to1(e) * 2.0 - 1.0,
			0.0f);
		sampleOffset.push_back(noise);
	}
	glGenTextures(1, &mOffset);
	glBindTexture(GL_TEXTURE_2D, mOffset);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, sampleOffset.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

SSAO::~SSAO()
{
	Clear();
}

void SSAO::Resize(int w, int h)
{
	Clear();
	Init(w, h);
}

void SSAO::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mOffset);

	mSSAOShd.Use();
	mScreen.Draw();
}

void SSAO::SetParm(const glm::mat4& vp, const glm::vec3& viewPos)
{
	mSSAOShd.Use();
	mSSAOShd.SetUniform("vp", vp);
	mSSAOShd.SetUniform("viewPos", viewPos);
}

void SSAO::SetResultTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mResult);
}

void SSAO::Init(int w, int h)
{
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenTextures(1, &mResult);
	glBindTexture(GL_TEXTURE_2D, mResult);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResult, 0);
}

void SSAO::Clear()
{
	glDeleteFramebuffers(1, &mFBO);
	glDeleteTextures(1, &mResult);
}
