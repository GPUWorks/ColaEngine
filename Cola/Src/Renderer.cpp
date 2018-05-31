#include "../Render/Renderer.h"

Renderer::Renderer(int w, int h) :
	mPrePass(128),
	mGeometryPass(w, h),
	mLightingPass(w, h),
	mUseSSAO(false),
	mSSAO(w, h),
	mUseBloom(true),
	mBloom(w, h),
	mUseDoF(false),
	mDoF(w, h)
{
	mPrePass.Render("../Assets/Images/CharlesRiver_Ref.hdr");
}

void Renderer::Resize(int w, int h)
{
	mGeometryPass.Resize(w, h);
	mLightingPass.Resize(w, h);
	mSSAO.Resize(w, h);
	mBloom.Resize(w, h);
	mDoF.Resize(w, h);
}

void Renderer::SetCamera(Camera* camera)
{
	auto view = camera->GetView();
	auto proj = camera->GetProj();
	auto viewPos = camera->GetPos();
	mGeometryPass.SetParm(proj * view, viewPos);
	mLightingPass.SetParm(view, proj, viewPos);
	mSSAO.SetParm(proj * view, viewPos);
}

void Renderer::AddToQueue(Model* model)
{
	mGeometryPass.mModels.emplace_back(model);
}

void Renderer::AddToQueue(Sphere * sphere)
{
	mGeometryPass.mSpheres.emplace_back(sphere);
}

void Renderer::Render()
{
	mGeometryPass.Render();

	mPrePass.SetSkyTexTo(0);
	mGeometryPass.SetGBufferTo(1, 2, 3, 4);
	mPrePass.SetDiffEnvTo(5);
	mPrePass.SetSpecEnvTo(6);
	mPrePass.SetBrdfLutTo(7);
	mLightingPass.Render();

	mLightingPass.SetResultTo(0);
	
	if (mUseSSAO) {
		mGeometryPass.SetGBufferTo(1, 2, 3, 4);
		mSSAO.Render();
		mSSAO.SetResultTo(0);
	}

	if (mUseBloom) {
		mBloom.Render();
		mBloom.SetResultTo(0);
	}

	if (mUseDoF) {
		mGeometryPass.SetGBufferTo(1, 2, 3, 4);
		mDoF.Render();
		mDoF.SetResultTo(0);
	}

	mGeometryPass.SetGBufferTo(1, 2, 3, 4);
	mFinalPass.Render();
}

void Renderer::LineModeSwitch(bool b)
{
	mGeometryPass.LineModeSwitch(b);
}

void Renderer::SSAOSwitch(bool b)
{
	mUseSSAO = b;
}

void Renderer::BloomSwitch(bool b)
{
	mUseBloom = b;
}

void Renderer::DoFSwitch(bool b)
{
	mUseDoF = b;
}

void Renderer::SetHDRI(const char* path)
{
	mPrePass.Render(path);
}

void Renderer::DebugView(int n)
{
	mFinalPass.DebugView(n);
}
