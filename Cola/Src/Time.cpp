#include "../Basic/Time.h"
#include <GLFW/glfw3.h>

double Time::mDeltaTime = 0.0, Time::mFrameTime = 0.0;

double Time::Delta()
{
	return mDeltaTime;
}

double Time::Frame()
{
	return mFrameTime;
}

double Time::Current()
{
	return glfwGetTime();
}
