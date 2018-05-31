#pragma once
#include "../Cola/Cola.h"
#include "../Cola/Scene/Scene.h"

class Demo : public Obj
{
public:
	Demo();
	void Update() override;
private:
	void HandleInput();
	void IMGUI();

	Scene mScene;
	std::shared_ptr<Model> mModel;
	std::shared_ptr<Sphere> mSphere;

	Transform mCamT0;
	Transform mCamT1;

	glm::vec3 mPos;
	glm::vec3 mRadius;
	glm::vec3 mPitch;
	float mYaw;
	// smooth
	glm::vec3 mCurrentPos;
	float mCurrentRadius;
	float mCurrentPitch;
	float mCurrentYaw;

	// gui
	bool mShowEditor, mShowInfo, mShowSetting;
	bool mShowHelp, mShowAbout;
	bool mVSync, mLineMode, mSSAO, mBloom, mDoF;

	// animation
	bool		mPlayAnim;
	float		mAnimTime;
	const char* mAnimName;
	int			mAnimIndex;
};