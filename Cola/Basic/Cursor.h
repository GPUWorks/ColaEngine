#pragma once

class Cursor
{
public:
	static float X();
	static float Y();
	static float DX();
	static float DY();
private:
	friend class Cola;
	static float mX, mY, mDX, mDY;
};