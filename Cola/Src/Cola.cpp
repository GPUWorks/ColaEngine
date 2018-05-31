#include "../Cola.h"
#include <glad/glad.h>
#pragma comment (lib, "glfw3.lib")
#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw_gl3.h"
#include "../Basic/Screen.h"
#include "../Basic/Cursor.h"
#include "../Basic/Time.h"
#include <iostream>

GLFWwindow* gWindow = nullptr;

Renderer* gRenderer = nullptr;

bool gKeepDrink = false;

std::vector<Obj*> gObjs;

void ErrorCallback(int error, const char* desc) {
	std::cout << desc << std::endl;
}

void ScreenSizeCallBack(GLFWwindow* window, int w, int h)
{
	if (h == 0) return;
	Screen::mWidth = w;
	Screen::mHeight = h;
	Screen::mAspect = static_cast<float>(w) / static_cast<float>(h);
	gRenderer->Resize(w, h);
	glViewport(0, 0, w, h);
}

Obj::Obj()
{
	mId = gObjs.size();
	gObjs.emplace_back(this);
}

Obj::~Obj()
{
	std::swap(gObjs[mId], gObjs.back());
	gObjs[mId]->mId = mId;
	gObjs.pop_back();
}

void Obj::Quit()
{
	gKeepDrink = false;
}

Renderer* Obj::GetRenderer()
{
	return gRenderer;
}

Cola::Cola()
{
	if (!glfwInit()) {
		std::cout << "glfw init failed!" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(ErrorCallback);
	const GLFWvidmode* modes = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int width = static_cast<int>(modes->width * 0.5f);
	int height = static_cast<int>(modes->height * 0.5f);
	gWindow = glfwCreateWindow(width, height, "Cola", nullptr, nullptr);
	if (gWindow == nullptr) {
		glfwTerminate();
		std::cout << "create window failed!" << std::endl;
		return;
	}
	glfwMakeContextCurrent(gWindow);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(gWindow, ScreenSizeCallBack);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "gladLoadGLLoader failed!" << std::endl;
		return;
	}
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(gWindow, true);
	ImGui::StyleColorsLight();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("../Assets/Fonts/msyh.ttc", 18, nullptr, io.Fonts->GetGlyphRangesChinese());
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 6.0f;
	style.GrabRounding = 6.0f;
	style.WindowRounding = 6.0f;
	style.ChildRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	ImGui_ImplGlfwGL3_NewFrame();

	gRenderer = new Renderer(width, height);
	ScreenSizeCallBack(nullptr, width, height);
	gKeepDrink = true;
}

Cola::~Cola()
{
	delete gRenderer;
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Cola::Drink()
{
	while (gKeepDrink && !glfwWindowShouldClose(gWindow)) {
		gRenderer->Render();
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(gWindow);
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		glm::dvec2 cursor;
		glfwGetCursorPos(glfwGetCurrentContext(), &cursor.x, &cursor.y);
		Cursor::mDX = static_cast<float>(cursor.x) - Cursor::mX;
		Cursor::mDY = static_cast<float>(cursor.y) - Cursor::mY;
		Cursor::mX += Cursor::mDX;
		Cursor::mY += Cursor::mDY;
		Time::mDeltaTime = glfwGetTime() - Time::mFrameTime;
		Time::mFrameTime += Time::mDeltaTime;
		for (auto obj : gObjs) {
			obj->Update();
		}
	}
}
