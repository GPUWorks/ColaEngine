#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vsPath, const char* fsPath);
	Shader(const char* vsPath, const char* fsPath, const char* tcsPath, const char* tesPath);
	~Shader();
	void Use();
	void SetUniform(const char* name, const int value);
	void SetUniform(const char* name, const float value);
	void SetUniform(const char* name, const double value);
	void SetUniform(const char* name, const glm::vec2& value);
	void SetUniform(const char* name, const glm::vec3& value);
	void SetUniform(const char* name, const glm::vec4& value);
	void SetUniform(const char* name, const glm::mat3& value);
	void SetUniform(const char* name, const glm::mat4& value);
	void SetUniformv(const char* name, unsigned int count, const glm::vec3* ptr);
	void SetUniformv(const char* name, unsigned int count, const glm::vec4* ptr);
	void SetUniformv(const char* name, unsigned int count, const glm::mat4* ptr);
private:
	unsigned int mId;
};