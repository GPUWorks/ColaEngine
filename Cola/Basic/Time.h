#pragma once

class Time
{
public:
	static double Delta();
	static double Frame();
	static double Current();
private:
	friend class Cola;
	static double mDeltaTime, mFrameTime;
};