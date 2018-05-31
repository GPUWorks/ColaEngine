#pragma once
#include "Pass/PrePass.h"
#include "Pass/GeometryPass.h"
#include "Pass/LightingPass.h"
#include "Pass/FinalPass.h"
#include "ImageEffect/SSAO.h"
#include "ImageEffect/Bloom.h"
#include "ImageEffect/DepthOfField.h"
#include "../Scene/Camera.h"

class Renderer
{
public:
	Renderer(int w, int h);
	void Resize(int w, int h);
	void SetCamera(Camera* camera);
	void AddToQueue(Model* model);
	void AddToQueue(Sphere* sphere);
	void Render();
	void LineModeSwitch(bool b);
	void SSAOSwitch(bool b);
	void BloomSwitch(bool b);
	void DoFSwitch(bool b);
	void SetHDRI(const char* path);
	void DebugView(int n);
private:
	PrePass mPrePass;
	GeometryPass mGeometryPass;
	LightingPass mLightingPass;
	FinalPass mFinalPass;

	bool mUseSSAO;
	SSAO mSSAO;
	bool mUseBloom;
	Bloom mBloom;
	bool mUseDoF;
	DepthOfField mDoF;
};