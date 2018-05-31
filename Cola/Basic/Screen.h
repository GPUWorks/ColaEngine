#pragma once

class Screen
{
public:
	static int Width();
	static int Height();
	static float Aspect();
private:
	friend void ScreenSizeCallBack(struct GLFWwindow*, int, int);
	static int mWidth, mHeight;
	static float mAspect;
};