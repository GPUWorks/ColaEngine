#pragma once

class HDRI
{
public:
	HDRI(const char* path);
	~HDRI();
	void SetTo(unsigned int slot);
private:
	unsigned int mId;
};