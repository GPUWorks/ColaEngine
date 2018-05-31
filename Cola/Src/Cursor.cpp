#include "../Basic/Cursor.h"

float Cursor::mX = 0.0f, Cursor::mY = 0.0f, Cursor::mDX = 0.f, Cursor::mDY = 0.0f;

float Cursor::X()
{
	return mX;
}

float Cursor::Y()
{
	return mY;
}

float Cursor::DX()
{
	return mDX;
}

float Cursor::DY()
{
	return mDY;
}
