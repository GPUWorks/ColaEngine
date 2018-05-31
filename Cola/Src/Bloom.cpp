#include "../Render/ImageEffect/Bloom.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>

Bloom::Bloom(int w, int h) :
	mHighLightShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/high_light_fs.glsl"),
	mBloomHShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/bloom_h_fs.glsl"),
	mBloomVShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/bloom_v_fs.glsl"),
	mBlendShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/bloom_blend_fs.glsl"),
	mScreen(gScreenVer, 4, gScreenInd, 6)
{
	Init(w, h);
	mBloomHShd.Use();
	mBloomHShd.SetUniform("image", 1);

	mBloomVShd.Use();
	mBloomVShd.SetUniform("image", 1);

	mBlendShd.Use();
	mBlendShd.SetUniform("bloom", 1);
}

Bloom::~Bloom()
{
	Clear();
}

void Bloom::Resize(int w, int h)
{
	Clear();
	Init(w, h);
}

void Bloom::Render()
{
	// 
	glBindFramebuffer(GL_FRAMEBUFFER, mHighLightFBO);
	mHighLightShd.Use();
	mScreen.Draw();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mHighLightTex);
	glGenerateMipmap(GL_TEXTURE_2D);
	// h
	glBindFramebuffer(GL_FRAMEBUFFER, mGaussFBO[0]);
	mBloomHShd.Use();
	mScreen.Draw();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mGaussBlur[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	// v
	glBindFramebuffer(GL_FRAMEBUFFER, mGaussFBO[1]);
	mBloomVShd.Use();
	mScreen.Draw();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mGaussBlur[1]);
	glGenerateMipmap(GL_TEXTURE_2D);
	// blend
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	mBlendShd.Use();
	mScreen.Draw();
}

void Bloom::SetResultTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mResult);
}

void Bloom::Init(int w, int h)
{
	
	glGenFramebuffers(1, &mHighLightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHighLightFBO);
	glGenTextures(1, &mHighLightTex);
	glBindTexture(GL_TEXTURE_2D, mHighLightTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mHighLightTex, 0);
	
	glGenFramebuffers(2, mGaussFBO);
	glGenTextures(2, mGaussBlur);

	glBindFramebuffer(GL_FRAMEBUFFER, mGaussFBO[0]);
	glBindTexture(GL_TEXTURE_2D, mGaussBlur[0]);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGaussBlur[0], 0
	);

	glBindFramebuffer(GL_FRAMEBUFFER, mGaussFBO[1]);
	glBindTexture(GL_TEXTURE_2D, mGaussBlur[1]);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGaussBlur[1], 0
	);

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &mResult);
	glBindTexture(GL_TEXTURE_2D, mResult);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResult, 0);
}

void Bloom::Clear()
{
	glDeleteFramebuffers(1, &mHighLightFBO);
	glDeleteTextures(1, &mHighLightTex);

	glDeleteFramebuffers(2, mGaussFBO);
	glDeleteTextures(2, mGaussBlur);

	glDeleteFramebuffers(1, &mFBO);
	glDeleteTextures(1, &mResult);
}
