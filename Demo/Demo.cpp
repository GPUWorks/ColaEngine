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
	mPlayAnim(false), mAnimTime(0.0f), mAnimName(u8"��ѡ�񶯻�"), mAnimIndex(999)
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
		// ������ƽ���
		if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)) {
			mPitch.x -= static_cast<float>(Cursor::DY() * 0.25);
			mPitch.x = glm::clamp(mPitch.x, mPitch.y, mPitch.z);
			mYaw -= static_cast<float>(Cursor::DX() * 0.5);
		}
		// �м�ƽ��
		else if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_MIDDLE)) {
			mPos += mCamT1.GetLocalAxisX() * -static_cast<float>(Cursor::DX()) * 0.01f;
			mPos += mCamT1.GetLocalAxisY() * static_cast<float>(Cursor::DY()) * 0.01f;
		}
		// ��������
		if (io.MouseWheel) {
			// ����λ�õ�����
			mRadius.x -= io.MouseWheel * 0.1f;
			mRadius.x = glm::clamp(mRadius.x, mRadius.y, mRadius.z);
		}
	}
	// ƽ��
	float deltaTime = static_cast<float>(Time::Delta());
	mCurrentRadius += (mRadius.x - mCurrentRadius) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentPitch += (mPitch.x - mCurrentPitch) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentYaw += (mYaw - mCurrentYaw) * glm::min(deltaTime / 0.1f, 1.0f);
	mCurrentPos += (mPos - mCurrentPos) * glm::min(deltaTime / 0.1f, 1.0f);
	// ��������任
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
	if (ImGui::BeginMenu(u8"�ļ�")) {
		if (ImGui::BeginMenu(u8"���ػ���")) {
			if (ImGui::MenuItem(u8"����0")) {
				GetRenderer()->SetHDRI("../Assets/Images/CharlesRiver_Ref.hdr");
			}
			if (ImGui::MenuItem(u8"����1")) {
				GetRenderer()->SetHDRI("../Assets/Images/Tropical_Beach_3k.hdr");
			}
			if (ImGui::MenuItem(u8"����2")) {
				GetRenderer()->SetHDRI("../Assets/Images/Mt-Washington-Gold-Room_Ref.hdr");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"����ģ��")) {
			if (ImGui::MenuItem(u8"ϸ������")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"��ѡ�񶯻�";
				mAnimIndex = 999;
				mModel.reset();
				mSphere = mScene.AddSphere();
			}
			if (ImGui::MenuItem(u8"ʾ��ģ��0(������)")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"��ѡ�񶯻�";
				mAnimIndex = 999;
				mSphere.reset();
				mModel = mScene.AddModel("../Assets/Models/0/1.fbx");
				mModel->mModelTrans.SetScaling(glm::vec3(0.01f, 0.01f, 0.01f));
			}
			if (ImGui::MenuItem(u8"ʾ��ģ��1(�޶���)")) {
				mPlayAnim = false;
				mAnimTime = 0.0f;
				mAnimName = u8"��ѡ�񶯻�";
				mAnimIndex = 999;
				mSphere.reset();
				mModel = mScene.AddModel("../Assets/Models/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");
				mModel->mModelTrans.SetScaling(glm::vec3(0.01f, 0.01f, 0.01f));
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if (ImGui::MenuItem(u8"�˳�")) Quit();
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"ѡ��")) {
		ImGui::MenuItem(u8"��ʾ�༭��", nullptr, &mShowEditor);
		ImGui::MenuItem(u8"��ʾ�������", nullptr, &mShowSetting);
		ImGui::MenuItem(u8"��ʾ������Ϣ", nullptr, &mShowInfo);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"��ͼ")) {
		if (ImGui::MenuItem(u8"����ɫ", nullptr)) {
			GetRenderer()->DebugView(-1);
		}
		if (ImGui::MenuItem(u8"���", nullptr)) {
			GetRenderer()->DebugView(0);
		}
		if (ImGui::MenuItem(u8"����ɫ", nullptr)) {
			GetRenderer()->DebugView(1);
		}
		if (ImGui::MenuItem(u8"����", nullptr)) {
			GetRenderer()->DebugView(2);
		}
		if (ImGui::MenuItem(u8"�ֲڶ�", nullptr)) {
			GetRenderer()->DebugView(3);
		}
		if (ImGui::MenuItem(u8"������", nullptr)) {
			GetRenderer()->DebugView(4);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"����")) {
		ImGui::MenuItem(u8"�ٿط�ʽ", nullptr, &mShowHelp);
		ImGui::MenuItem(u8"����...", nullptr, &mShowAbout);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	//
	if (mShowSetting) {
		ImGui::Begin(u8"�������", &mShowSetting, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

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
		ImGui::Begin(u8"������Ϣ", &mShowInfo, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		ImGui::Text(u8"������Ϣ");
		ImGui::Separator();
		ImGui::Text("FPS: %.1f(use %.3f ms per frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::End();
	}
	//
	if (mShowHelp) {
		ImGui::Begin(u8"�ٿط�ʽ", &mShowHelp, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text(u8"������+�ƶ�: ��������");
		ImGui::Text(u8"����м�: ƽ�����");
		ImGui::Text(u8"������: ���ž���");
		ImGui::End();
	}
	if (mShowAbout) {
		ImGui::Begin(u8"����", &mShowAbout, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text(u8"����汾��0.1.0");
		ImGui::Separator();
		ImGui::Text(u8"�������ߣ�Neyco");
		ImGui::End();
	}

	if (mShowEditor) {
		
		if (mSphere) {
			ImGui::Begin(u8"�༭��", &mShowEditor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Tessellation Sphere");
			ImGui::Separator();
			ImGui::SliderFloat(u8"�뾶", &mSphere->mAttributes.mRadius, 0.0f, 10.0f);
			ImGui::SliderFloat(u8"ϸ�ֵȼ�", &mSphere->mAttributes.mTessLevel, 0.0f, 32.0f);
			ImGui::SliderFloat(u8"�ֲڶ�", &mSphere->mAttributes.mRoughness, 0.0f, 1.0f);
			ImGui::SliderFloat(u8"������", &mSphere->mAttributes.mMetallic, 0.0f, 1.0f);
			ImGui::ColorPicker3(u8"����ɫ", glm::value_ptr(mSphere->mAttributes.mAlbedo));
			ImGui::End();
		}
		if (mModel) {
			ImGui::Begin(u8"�༭��", &mShowEditor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
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
				if (ImGui::Button(u8"  ��ͣ  ")) mPlayAnim = false;
			}
			else {
				if (ImGui::Button(u8"  ����  ")) mPlayAnim = true;
			}
			ImGui::SliderFloat(u8"����ʱ��", &mAnimTime, 0.0f, static_cast<float>(mModel->GetAnimDuration(mAnimIndex)));
			ImGui::Separator();
			ImGui::End();
		}
	}
	
}

