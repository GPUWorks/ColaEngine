#pragma once
#include "../Render/Renderer.h"
#include "Camera.h"
#include "Model.h"
#include <vector>
#include <memory>

class Scene
{
public:
	Scene(Renderer* renderer);
	void Update();
	std::shared_ptr<Model> AddModel(const char* path);
	std::shared_ptr<Sphere> AddSphere();
	Camera mCamera;
private:
	Renderer* mRenderer;
	std::vector<Model*> mModels;
	std::vector<Sphere*> mSpheres;
};