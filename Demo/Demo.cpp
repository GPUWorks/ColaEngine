#include "Demo.h"
#include "../Cola/Basic/Cursor.h"
#include "../Cola/Basic/Time.h"
#include <GLFW/glfw3.h>
#include "../Cola/imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

Demo::Demo() : mScene(GetRenderer()),
	mPos(0.0f), mRadius(2.0f, 0.1f, 5.0f),
	mPitch(0.0f, -80.0f, 80.0f), mYaw(0.0f),
	mCurrentPos(mPos), mCurrentRadius(mRadius.x),
	mCurrentPitch(mPitch.x), mCurrentYaw(mYaw),
	mShowEditor(true), mShowInfo(true), mShowSetting(true),
	mShowHelp(false), mShowAbout(false),
	mVSync(true), mLineMode(false), mSSAO(false), mBloom(true), mDoF(false),
	mPlayAnim(false), mAnimTime(0.0f), mAnimName(u8"请选择动画"), mAnimIndex(999)
{
	mCamT0.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	mSphere = mScene.AddSphere();
}

void Demo::Update()
{
	HandleInput();
	IMGUI();
	if (mPlayAnim) {
		if (mModel) {
			mAnimTime = static_cast<float>(glm::mod(
				mAnimTime + Time::Delta(),
				mModel->GetAnimDuration(mAnimIndex)
			));
		}
	}
	if (mModel) mModel->UpdateAnim(mAnimIndex, mAnimTime);
}

void Demo::HandleInput()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse) {
		// 左键环绕焦点
		if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)) {
			mPitch.x -= static_cast<float>(Cursor::DY() * 0.25);
			mPitch.x = glm::clamp(mPitch.x, mPitch.y, mPitch.z);
			mYaw -= static_cast<float>(Cursor::DX() * 0.5);
		}
		// 中键平移
		else if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_MIDDLE)) {
			mPos += mCamT1.GetLocalAxisX() * -static_cast<float>(Cursor::DX()) * 0.01f;
			mPos += mCamT1.GetLocalAxisY() * static_cast<float>(Cursor::DY()) * 0.01f;
		}
		// 滚轮缩放
		if (io.MouseWheel) {
			// 基于位置的缩放
			mRadius.x -= io.MouseWheel * 0.1f;
			mRadius.x = glm::clamp(mRadius.x, mRadius.y, mRadius.z);
		}
	}
	// 平滑
	float deltaTime = static_cast<float>(Time::Delta());
	mCurrentRadius += (mRadius.x - mCurrentRadius) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentPitch += (mPitch.x - mCurrentPitch) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentYaw += (mYaw - mCurrentYaw) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentPos += (mPos - mCurrentPos) * glm::min(deltaTime / 0.1f, 1.0f);
	// 生成相机变换
	mCamT0.SetPosition(glm::vec3(0, 0, mCurrentRadius));
	mCamT1.SetRotationEuler(glm::vec3(glm::radians(mCurrentPitch), glm::radians(glm::mod(mCurrentYaw, 360.0f)), 0.0f));
	mCamT1.SetPosition(mCurrentPos);

	mScene.mCamera.mTransform.SetMatrix(mCamT1.GetMatrix() * mCamT0.GetMatrix());
	mScene.Update();
}

void Demo::IMGUI()
{
	//
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu(u8"文件")) {
		if (ImGui::BeginMenu(u8"加载环境")) {
			if (ImGui::MenuItem(u8"环境0")) {
				GetRenderer()->SetHDRI("../Assets/Images/CharlesRiver_Ref.hdr");
			}
			if (ImGui::MenuItem(u8"环境1")) {
				GetRenderer()->SetHDRI("../Assets/Images/Tropical_Beach_3k.hdr");
			}
			if (ImGui::MenuItem(u8"环境2")) {
				GetRenderer()->SetHDRI("../Assets/Images/Mt-Washington-Gold-Room_Ref.hdr");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"加载模型")) {
			if (ImGui::MenuItem(u8"细分球体")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"请选择动画";
				mAnimIndex = 999;
				mModel.reset();
				mSphere = mScene.AddSphere();
			}
			if (ImGui::MenuItem(u8"示例模型0(带动画)")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"请选择动画";
				mAnimIndex = 999;
				mSphere.reset();
				mModel = mScene.AddModel("../Assets/Models/0/1.fbx");
				mModel->mModelTrans.SetScaling(glm::vec3(0.01f, 0.01f, 0.01f));
			}
			if (ImGui::MenuItem(u8"示例模型1(无动画)")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"请选择动画";
				mAnimIndex = 999;
				mSphere.reset();
				mModel = mScene.AddModel("../Assets/Models/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");
				mModel->mModelTrans.SetScaling(glm::vec3(0.01f, 0.01f, 0.01f));
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if (ImGui::MenuItem(u8"退出")) Quit();
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"选项")) {
		ImGui::MenuItem(u8"显示编辑器", nullptr, &mShowEditor);
		ImGui::MenuItem(u8"显示设置面板", nullptr, &mShowSetting);
		ImGui::MenuItem(u8"显示引擎信息", nullptr, &mShowInfo);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"视图")) {
		if (ImGui::MenuItem(u8"最终色", nullptr)) {
			GetRenderer()->DebugView(-1);
		}
		if (ImGui::MenuItem(u8"深度", nullptr)) {
			GetRenderer()->DebugView(0);
		}
		if (ImGui::MenuItem(u8"固有色", nullptr)) {
			GetRenderer()->DebugView(1);
		}
		if (ImGui::MenuItem(u8"法线", nullptr)) {
			GetRenderer()->DebugView(2);
		}
		if (ImGui::MenuItem(u8"粗糙度", nullptr)) {
			GetRenderer()->DebugView(3);
		}
		if (ImGui::MenuItem(u8"金属度", nullptr)) {
			GetRenderer()->DebugView(4);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"帮助")) {
		ImGui::MenuItem(u8"操控方式", nullptr, &mShowHelp);
		ImGui::MenuItem(u8"关于...", nullptr, &mShowAbout);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	//
	if (mShowSetting) {
		ImGui::Begin(u8"设置面板", &mShowSetting, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Checkbox("V-Sync", &mVSync)) {
			if (mVSync) glfwSwapInterval(1);
			else glfwSwapInterval(0);
		}
		if (ImGui::Checkbox("Line mode", &mLineMode)) {
			GetRenderer()->LineModeSwitch(mLineMode);
		}
		if (ImGui::Checkbox("SSAO", &mSSAO)) {
			GetRenderer()->SSAOSwitch(mSSAO);
		}
		if (ImGui::Checkbox("Bloom", &mBloom)) {
			GetRenderer()->BloomSwitch(mBloom);
		}
		if (ImGui::Checkbox("DoF", &mDoF)) {
			GetRenderer()->DoFSwitch(mDoF);
		}
		ImGui::End();
	}
	//
	if (mShowInfo) {
		ImVec2 pos = ImVec2(5.0f, ImGui::GetIO().DisplaySize.y - 5.0f);
		ImVec2 pivot = ImVec2(0.0f, 1.0f);
		ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
		ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
		ImGui::Begin(u8"引擎信息", &mShowInfo, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		ImGui::Text(u8"引擎信息");
		ImGui::Separator();
		ImGui::Text("FPS: %.1f(use %.3f ms per frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::End();
	}
	//
	if (mShowHelp) {
		ImGui::Begin(u8"操控方式", &mShowHelp, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text(u8"鼠标左键+移动: 环绕物体");
		ImGui::Text(u8"鼠标中键: 平移相机");
		ImGui::Text(u8"鼠标滚轮: 缩放距离");
		ImGui::End();
	}
	if (mShowAbout) {
		ImGui::Begin(u8"关于", &mShowAbout, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text(u8"引擎版本：0.1.0");
		ImGui::Separator();
		ImGui::Text(u8"引擎作者：Neyco");
		ImGui::End();
	}

	if (mShowEditor) {
		
		if (mSphere) {
			ImGui::Begin(u8"编辑器", &mShowEditor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Tessellation Sphere");
			ImGui::Separator();
			ImGui::SliderFloat(u8"半径", &mSphere->mAttributes.mRadius, 0.0f, 10.0f);
			ImGui::SliderFloat(u8"细分等级", &mSphere->mAttributes.mTessLevel, 0.0f, 32.0f);
			ImGui::SliderFloat(u8"粗糙度", &mSphere->mAttributes.mRoughness, 0.0f, 1.0f);
			ImGui::SliderFloat(u8"金属度", &mSphere->mAttributes.mMetallic, 0.0f, 1.0f);
			ImGui::ColorPicker3(u8"固有色", glm::value_ptr(mSphere->mAttributes.mAlbedo));
			ImGui::End();
		}
		if (mModel) {
			ImGui::Begin(u8"编辑器", &mShowEditor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Model");
			ImGui::Separator();
			const char** anims = new const char*[mModel->GetAnimCount()];
			for (int i(0); i < mModel->GetAnimCount(); ++i) {
				anims[i] = mModel->GetAnimName(i);
			}

			if (ImGui::BeginCombo("", mAnimName))
			{
				for (int n = 0; n < mModel->GetAnimCount(); ++n)
				{
					bool is_selected = (mAnimName == anims[n]);
					if (ImGui::Selectable(anims[n], is_selected)) {
						mAnimName = anims[n];
						mAnimIndex = n;
						mAnimTime = 0.0f;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (mPlayAnim) {
				if (ImGui::Button(u8"  暂停  ")) mPlayAnim = false;
			}
			else {
				if (ImGui::Button(u8"  播放  ")) mPlayAnim = true;
			}
			ImGui::SliderFloat(u8"动画时间", &mAnimTime, 0.0f, static_cast<float>(mModel->GetAnimDuration(mAnimIndex)));
			ImGui::Separator();
			ImGui::End();
		}
	}
	
}

