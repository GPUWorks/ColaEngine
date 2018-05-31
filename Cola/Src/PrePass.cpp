#include "../Render/Pass/PrePass.h"
#include "../Basic/Shape.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PrePass::PrePass(int resolution) :
	mDiffEnvShd("../Assets/Shaders/diff_env_vs.glsl", "../Assets/Shaders/diff_env_fs.glsl"),
	mSpecEnvShd("../Assets/Shaders/spec_env_vs.glsl", "../Assets/Shaders/spec_env_fs.glsl"),
	mBrdfLutShd("../Assets/Shaders/brdf_lut_vs.glsl", "../Assets/Shaders/brdf_lut_fs.glsl"),
	mSkyCube(gCubeVer, 24, gSkyCubeInd, 36),
	mScreen(gScreenVer, 4, gScreenInd, 6),
	mRes(resolution)
{
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	mViewProj[0] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mViewProj[1] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mViewProj[2] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mViewProj[3] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	mViewProj[4] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mViewProj[5] = proj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	// diff
	glGenFramebuffers(1, &mDiffEnvFBO);
	glGenTextures(1, &mDiffEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mDiffEnvMap);
	for (unsigned int i(0); i < 6; ++i)
	{
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// spec
	glGenFramebuffers(1, &mSpecEnvFBO);
	glGenTextures(1, &mSpecEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSpecEnvMap);
	for (unsigned int i(0); i < 6; ++i)
	{
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	// brdf
	glGenFramebuffers(1, &mBrdfLutFBO);
	glGenTextures(1, &mBrdfLutTex);
	glBindTexture(GL_TEXTURE_2D, mBrdfLutTex);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RG16F,
		resolution, resolution, 0, GL_RG, GL_FLOAT, nullptr
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

PrePass::~PrePass()
{
	delete mEnvHDRI;
	glDeleteFramebuffers(1, &mDiffEnvFBO);
	glDeleteFramebuffers(1, &mSpecEnvFBO);
	glDeleteFramebuffers(1, &mBrdfLutFBO);
	glDeleteTextures(1, &mDiffEnvMap);
	glDeleteTextures(1, &mSpecEnvMap);
	glDeleteTextures(1, &mBrdfLutTex);
}

void PrePass::Render(const char* hdri)
{
	// 暂存视口用于恢复
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	mEnvHDRI = new HDRI(hdri);
	mEnvHDRI->SetTo(0);
	// diff
	glViewport(0, 0, mRes, mRes);
	glBindFramebuffer(GL_FRAMEBUFFER, mDiffEnvFBO);
	mDiffEnvShd.Use();
	for (unsigned int n(0); n < 6; ++n) {
		mDiffEnvShd.SetUniform("vp", mViewProj[n]);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + n, mDiffEnvMap, 0
		);
		mSkyCube.Draw();
		glFinish();
	}
	// spec
	glBindFramebuffer(GL_FRAMEBUFFER, mSpecEnvFBO);
	mSpecEnvShd.Use();
	int mipCount = 5;
	for (int mipLevel(0); mipLevel < mipCount; ++mipLevel) {
		float roughness = static_cast<float>(mipLevel) / static_cast<float>(mipCount - 1);
		mSpecEnvShd.SetUniform("roughness", roughness);
		for (unsigned int n(0); n < 6; ++n) {
			mSpecEnvShd.SetUniform("vp", mViewProj[n]);
			int res = static_cast<int>(mRes * glm::pow(0.5, mipLevel));
			glViewport(0, 0, res, res);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + n, mSpecEnvMap, mipLevel
			);
			mSkyCube.Draw();
			glFinish();
		}
	}
	// brdf
	glViewport(0, 0, mRes, mRes);
	glBindFramebuffer(GL_FRAMEBUFFER, mBrdfLutFBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, mBrdfLutTex, 0
	);
	mBrdfLutShd.Use();
	mScreen.Draw();
	glFinish();

	glViewport(0, 0, viewport[2], viewport[3]);
}

void PrePass::SetSkyTexTo(unsigned int slot)
{
	if (mEnvHDRI) mEnvHDRI->SetTo(slot);
}

void PrePass::SetDiffEnvTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mDiffEnvMap);
}

void PrePass::SetSpecEnvTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSpecEnvMap);
}

void PrePass::SetBrdfLutTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mBrdfLutTex);
}
