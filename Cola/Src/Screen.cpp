#include "../Basic/Screen.h"

int Screen::mWidth, Screen::mHeight;
float Screen::mAspect;

int Screen::Width()
{
	return mWidth;
}

int Screen::Height()
{
	return mHeight;
}

float Screen::Aspect()
{
	return mAspect;
}
