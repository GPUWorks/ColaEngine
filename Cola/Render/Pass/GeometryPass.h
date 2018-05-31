#pragma once
#include "../Shader.h"
#include "../../Scene/Model.h"
#include "../../Scene/Sphere.h"
#include <vector>

class GeometryPass
{
public:
	GeometryPass(int w, int h);
	~GeometryPass();
	void Resize(int w, int h);
	void SetParm(const glm::mat4& vp, const glm::vec3& viewPos);
	void Render();
	void SetGBufferTo(unsigned int posDepth, unsigned int normalAO, unsigned int albedoRoughness, unsigned int emissiveMetallic);
	void LineModeSwitch(bool b);
	std::vector<Model*> mModels;
	std::vector<Sphere*> mSpheres;
private:
	void Init(int w, int h);
	void Clear();
	Shader mModelShd;
	Shader mSphereShd;
	unsigned int mFBO;
	unsigned int mDepthRBO;
	unsigned int mPosDepth;
	unsigned int mNormalAO;
	unsigned int mAlbedoRoughness;
	unsigned int mEmissiveMetallic;

	glm::mat4 mVP;

	unsigned int mDrawMode;
};