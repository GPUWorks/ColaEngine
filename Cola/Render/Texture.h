#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();
	void SetTo(unsigned int slot);
private:
	unsigned int mId;
};