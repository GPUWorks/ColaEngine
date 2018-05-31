#include "../Render/ImageEffect/DepthOfField.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>

DepthOfField::DepthOfField(int w, int h) :
	mCoCShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/dof_coc_fs.glsl"),
	mBlurHShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/dof_blur_h_fs.glsl"),
	mBlurVShd("../Assets/Shaders/screen_vs.glsl", "../Assets/Shaders/dof_blur_v_fs.glsl"),
	mScreen(gScreenVer, 4, gScreenInd, 6)
{
	Init(w, h);

	mCoCShd.Use();
	mCoCShd.SetUniform("PosDepth", 1);
	mCoCShd.SetUniform("foregroundRange", glm::vec2(0.1f, 0.5f));
	mCoCShd.SetUniform("backgroundRange", glm::vec2(2.0f, 5.0f));

	mBlurHShd.Use();
	mBlurHShd.SetUniform("coc", 1);

	mBlurVShd.Use();
	mBlurVShd.SetUniform("coc", 1);
}

DepthOfField::~DepthOfField()
{
	Clear();
}

void DepthOfField::Resize(int w, int h)
{
	Clear();
	Init(w, h);
}

void DepthOfField::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mCoCFBO);
	mCoCShd.Use();
	mScreen.Draw();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mCoC);

	glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[0]);
	mBlurHShd.Use();
	mScreen.Draw();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mBlur[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[1]);
	mBlurVShd.Use();
	mScreen.Draw();
}

void DepthOfField::SetResultTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mBlur[1]);
}

void DepthOfField::Init(int w, int h)
{
	glGenFramebuffers(1, &mCoCFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mCoCFBO);
	glGenTextures(1, &mCoC);
	glBindTexture(GL_TEXTURE_2D, mCoC);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mCoC, 0);

	glGenFramebuffers(2, mBlurFBO);
	glGenTextures(2, mBlur);

	glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[0]);
	glBindTexture(GL_TEXTURE_2D, mBlur[0]);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlur[0], 0
	);

	glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[1]);
	glBindTexture(GL_TEXTURE_2D, mBlur[1]);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlur[1], 0
	);
}

void DepthOfField::Clear()
{
	glDeleteFramebuffers(1, &mCoCFBO);
	glDeleteTextures(1, &mCoC);

	glDeleteFramebuffers(2, mBlurFBO);
	glDeleteTextures(2, mBlur);
}
