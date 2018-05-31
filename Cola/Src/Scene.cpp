#include "../Scene/Scene.h"
#include "../Basic/Screen.h"

Scene::Scene(Renderer* renderer) :
	mRenderer(renderer),
	mCamera(Screen::Aspect(), 75.0f)
{
}

void Scene::Update()
{
	mCamera.SetAspect(Screen::Aspect());
	mRenderer->SetCamera(&mCamera);
	for (auto& model : mModels) {
		mRenderer->AddToQueue(model);
	}
	for (auto& sphere : mSpheres) {
		mRenderer->AddToQueue(sphere);
	}
}

std::shared_ptr<Model> Scene::AddModel(const char* path)
{
	std::shared_ptr<Model> m(new Model(path),
		[&](Model* m) {
		std::swap(mModels[m->mId], mModels.back());
		mModels[m->mId]->mId = m->mId;
		mModels.pop_back(); delete m;
	}
	);
	m->mId = mModels.size();
	mModels.emplace_back(m.get());
	return m;
}

std::shared_ptr<Sphere> Scene::AddSphere()
{
	std::shared_ptr<Sphere> s(new Sphere(),
		[&](Sphere* s) {
		std::swap(mSpheres[s->mId], mSpheres.back());
		mSpheres[s->mId]->mId = s->mId;
		mSpheres.pop_back(); delete s;
	}
	);
	s->mId = mSpheres.size();
	mSpheres.emplace_back(s.get());
	return s;
}
