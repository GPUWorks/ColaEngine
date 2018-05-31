#include "../Render/HDRI.h"

#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>

HDRI::HDRI(const char* path)
{
	std::cout << "Load hdr image:\n" << path << "\n";
	// gen
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	// settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load file
	int width, height, channels;
	float* data = stbi_loadf(path, &width, &height, &channels, 0);
	if (!data) {
		std::cout << "Load hdr image failed!" << std::endl;
		return;
	}
	int type;
	switch (channels)
	{
	case 3: type = GL_RGB; break;
	case 4: type = GL_RGBA; break;
	default: std::cout << "Error hdr image type!" << std::endl; stbi_image_free(data); return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, type, GL_FLOAT, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

HDRI::~HDRI()
{
	glDeleteTextures(1, &mId);
}

void HDRI::SetTo(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mId);
}
