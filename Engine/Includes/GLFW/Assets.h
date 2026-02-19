#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../AssetsHelper/Asset.h"

inline unsigned int GetShader(const char* path, GLenum type) {
	int id = glCreateShader(type);
	std::ifstream file(path);

	std::stringstream buffer;
	buffer << file.rdbuf();

	std::string src = buffer.str();
	const char* srcPtr = src.c_str();
	int length = static_cast<int>(src.length());

	glShaderSource(id, 1, &srcPtr, &length);
	glCompileShader(id);

	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n %s\n", infoLog);
	};

	return id;
}

inline void TestShaderProgram(unsigned int id) {
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}
}

class Shader : public Asset {
public:
	unsigned int Id;
	Shader(const char* vertPath, const char* fragPath) {
		unsigned vertex, fragment;
		vertex = GetShader(vertPath, GL_VERTEX_SHADER);
		fragment = GetShader(fragPath, GL_FRAGMENT_SHADER);
		Id = glCreateProgram();
		glAttachShader(Id, vertex);
		glAttachShader(Id, fragment);
		glLinkProgram(Id);
		TestShaderProgram(Id);
		glDetachShader(Id, vertex);
		glDetachShader(Id, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};
	Shader(const char* vertPath, const char* geomPath, const char* fragPath) {
		unsigned vertex, geometric, fragment;
		vertex = GetShader(vertPath, GL_VERTEX_SHADER);
		geometric = GetShader(geomPath, GL_GEOMETRY_SHADER);
		fragment = GetShader(fragPath, GL_FRAGMENT_SHADER);
		Id = glCreateProgram();
		glAttachShader(Id, vertex);
		glAttachShader(Id, geometric);
		glAttachShader(Id, fragment);
		glLinkProgram(Id);
		TestShaderProgram(Id);
		glDetachShader(Id, vertex);
		glDetachShader(Id, geometric);
		glDetachShader(Id, fragment);

		glDeleteShader(vertex);
		glDeleteShader(geometric);
		glDeleteShader(fragment);
	};
	Shader(const char* compPath) {
		unsigned compute;
		compute = GetShader(compPath, GL_COMPUTE_SHADER);
		Id = glCreateProgram();
		glAttachShader(Id, compute);
		glLinkProgram(Id);
		TestShaderProgram(Id);
		glDetachShader(Id, compute);

		glDeleteShader(compute);
	}
	Shader() : Id(0) {};
	~Shader() override {
		glDeleteProgram(Id);
	};
	unsigned int GetUniformId(std::string name);

private:
	std::unordered_map<std::string, unsigned int> uniformIds;
};