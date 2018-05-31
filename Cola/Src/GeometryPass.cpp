#include "../Render/Pass/GeometryPass.h"
#include <glad/glad.h>

GeometryPass::GeometryPass(int w, int h) :
	mModelShd("../Assets/Shaders/model_vs.glsl", "../Assets/Shaders/model_fs.glsl"),
	mSphereShd(
		"../Assets/Shaders/tess_sphere_vs.glsl",
		"../Assets/Shaders/tess_sphere_fs.glsl",
		"../Assets/Shaders/tess_sphere_tcs.glsl",
		"../Assets/Shaders/tess_sphere_tes.glsl"
	),
	mDrawMode(GL_FILL)
{
	mModelShd.Use();
	mModelShd.SetUniform("albedoTex", 0);
	mModelShd.SetUniform("normalTex", 1);
	mModelShd.SetUniform("roughnessTex", 2);
	mModelShd.SetUniform("metallicTex", 3);
	mModelShd.SetUniform("emissiveTex", 4);
	Init(w, h);
}

GeometryPass::~GeometryPass()
{
	Clear();
}

void GeometryPass::Resize(int w, int h)
{
	Clear();
	Init(w, h);
}

void GeometryPass::SetParm(const glm::mat4& vp, const glm::vec3& viewPos)
{
	mVP = vp;
	mModelShd.Use();
	mModelShd.SetUniform("viewPos", viewPos);
	mSphereShd.Use();
	mSphereShd.SetUniform("viewPos", viewPos);
}

void GeometryPass::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, mDrawMode);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mModelShd.Use();
	for (auto& model : mModels) {
		mModelShd.SetUniformv(
			"boneTrans",
			model->mBoneFinalTrans.size(),
			model->mBoneFinalTrans.data()
		);
		for (size_t i(0); i < model->mMeshes.size(); ++i) {
			mModelShd.SetUniform("model", model->mModelTrans.GetMatrix() * model->mMeshTrans[i].GetMatrix());
			mModelShd.SetUniform("mvp", mVP * model->mModelTrans.GetMatrix() * model->mMeshTrans[i].GetMatrix());
			model->mAlbedos[i]->SetTo(0);
			model->mNormals[i]->SetTo(1);
			model->mRoughness[i]->SetTo(2);
			model->mMetallics[i]->SetTo(3);
			model->mEmissive[i]->SetTo(4);
			model->mMeshes[i]->Draw();
		}
	}
	mModels.clear();

	mSphereShd.Use();
	for (auto& sphere : mSpheres) {
		mSphereShd.SetUniform("model", sphere->mTransform.GetMatrix());
		mSphereShd.SetUniform("mvp", mVP * sphere->mTransform.GetMatrix());
		mSphereShd.SetUniform("radius", sphere->mAttributes.mRadius);
		mSphereShd.SetUniform("tessLevel", sphere->mAttributes.mTessLevel);
		mSphereShd.SetUniform("albedo", sphere->mAttributes.mAlbedo);
		mSphereShd.SetUniform("emissive", sphere->mAttributes.mEmissive);
		mSphereShd.SetUniform("roughness", sphere->mAttributes.mRoughness);
		mSphereShd.SetUniform("metallic", sphere->mAttributes.mMetallic);
		sphere->mMesh.DrawPatches4();
	}
	mSpheres.clear();

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GeometryPass::SetGBufferTo(unsigned int posDepth, unsigned int normalAO, unsigned int albedoRoughness, unsigned int emissiveMetallic)
{
	glActiveTexture(GL_TEXTURE0 + posDepth);
	glBindTexture(GL_TEXTURE_2D, mPosDepth);
	glActiveTexture(GL_TEXTURE0 + normalAO);
	glBindTexture(GL_TEXTURE_2D, mNormalAO);
	glActiveTexture(GL_TEXTURE0 + albedoRoughness);
	glBindTexture(GL_TEXTURE_2D, mAlbedoRoughness);
	glActiveTexture(GL_TEXTURE0 + emissiveMetallic);
	glBindTexture(GL_TEXTURE_2D, mEmissiveMetallic);
}

void GeometryPass::LineModeSwitch(bool b)
{
	if (b) mDrawMode = GL_LINE;
	else mDrawMode = GL_FILL;
}

void GeometryPass::Init(int w, int h)
{
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	// Depth
	glGenRenderbuffers(1, &mDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);
	// Position Buffer
	glGenTextures(1, &mPosDepth);
	glBindTexture(GL_TEXTURE_2D, mPosDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPosDepth, 0);
	// Normal Buffer
	glGenTextures(1, &mNormalAO);
	glBindTexture(GL_TEXTURE_2D, mNormalAO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalAO, 0);
	// AlbedoRoughness Buffer
	glGenTextures(1, &mAlbedoRoughness);
	glBindTexture(GL_TEXTURE_2D, mAlbedoRoughness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mAlbedoRoughness, 0);
	// EmissiveMetallic Buffer
	glGenTextures(1, &mEmissiveMetallic);
	glBindTexture(GL_TEXTURE_2D, mEmissiveMetallic);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mEmissiveMetallic, 0);
	// Attachments
	GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);
}

void GeometryPass::Clear()
{
	glDeleteFramebuffers(1, &mFBO);
	glDeleteRenderbuffers(1, &mDepthRBO);
	glDeleteTextures(1, &mPosDepth);
	glDeleteTextures(1, &mNormalAO);
	glDeleteTextures(1, &mAlbedoRoughness);
	glDeleteTextures(1, &mEmissiveMetallic);
}
