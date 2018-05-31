#include "../Render/Shader.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// 注意该函数返回值的有效期在下一次调用时终止
std::string& StringFromFile(const char* path)
{
	static std::string str;
	str.clear();
	std::ifstream file(path);
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	str = stream.str();
	return str;
}

unsigned int CompileShader(unsigned int type, const char* const* code)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, code, nullptr);
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Compile shader failed!\n" << infoLog;
	};
	return shader;
}

unsigned int LinkShader(unsigned int vs, unsigned int fs)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "Link shader failed!\n" << infoLog;
	}
	return program;
}

unsigned int LinkShader(unsigned int vs, unsigned int fs, unsigned int tcs, unsigned int tes)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glLinkProgram(program);
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "Link shader failed!\n" << infoLog;
	}
	return program;
}

Shader::Shader(const char* vsPath, const char* fsPath)
{
	std::cout << "Load shader:\n" << vsPath << "\n" << fsPath << "\n";
	// vs
	const char* vsCode = StringFromFile(vsPath).c_str();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, &vsCode);
	// fs
	const char* fsCode = StringFromFile(fsPath).c_str();
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, &fsCode);
	// link
	mId = LinkShader(vs, fs);
	// clear
	glDeleteProgram(vs);
	glDeleteProgram(fs);
}

Shader::Shader(const char* vsPath, const char* fsPath, const char* tcsPath, const char* tesPath)
{
	std::cout << "Load shader:\n" << vsPath << "\n" << fsPath << "\n" << tcsPath << "\n" << tesPath << "\n";
	// vs
	const char* vsCode = StringFromFile(vsPath).c_str();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, &vsCode);
	// fs
	const char* fsCode = StringFromFile(fsPath).c_str();
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, &fsCode);
	// tcs
	const char* tcsCode = StringFromFile(tcsPath).c_str();
	unsigned int tcs = CompileShader(GL_TESS_CONTROL_SHADER, &tcsCode);
	// tes
	const char* tesCode = StringFromFile(tesPath).c_str();
	unsigned int tes = CompileShader(GL_TESS_EVALUATION_SHADER, &tesCode);
	// link
	mId = LinkShader(vs, fs, tcs, tes);
	// clear
	glDeleteProgram(vs);
	glDeleteProgram(fs);
	glDeleteProgram(tcs);
	glDeleteProgram(tes);
}

Shader::~Shader()
{
	glDeleteProgram(mId);
}

void Shader::Use()
{
	glUseProgram(mId);
}

void Shader::SetUniform(const char* name, const int value)
{
	glUniform1i(glGetUniformLocation(mId, name), value);
}

void Shader::SetUniform(const char* name, const float value)
{
	glUniform1f(glGetUniformLocation(mId, name), value);
}

void Shader::SetUniform(const char* name, const double value)
{
	glUniform1d(glGetUniformLocation(mId, name), value);
}

void Shader::SetUniform(const char* name, const glm::vec2& value)
{
	glUniform2f(glGetUniformLocation(mId, name), value.x, value.y);
}

void Shader::SetUniform(const char* name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(mId, name), value.x, value.y, value.z);
}

void Shader::SetUniform(const char* name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(mId, name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const char* name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformv(const char* name, unsigned int count, const glm::vec3* ptr)
{
	glUniform3fv(glGetUniformLocation(mId, name), count, (float*)ptr);
}

void Shader::SetUniformv(const char* name, unsigned int count, const glm::vec4* ptr)
{
	glUniform4fv(glGetUniformLocation(mId, name), count, (float*)ptr);
}

void Shader::SetUniformv(const char* name, unsigned int count, const glm::mat4* ptr)
{
	glUniformMatrix4fv(glGetUniformLocation(mId, name), count, GL_FALSE, (float*)ptr);
}
