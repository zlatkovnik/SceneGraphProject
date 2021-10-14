#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
private:
	GLuint m_id;
public:
	static std::unordered_map<std::string, Shader*> ShaderMap;

	Shader(const char* vertexFile, const char* fragmentFile);
	void Bind();

	void SetInt(const char* uniformName, int value);
	void SetFloat(const char* uniformName, float value);
	void SetMat4(const char* uniformName, glm::mat4& mat);
private:
	void CompileErrors(unsigned int shader, const char* type);
};