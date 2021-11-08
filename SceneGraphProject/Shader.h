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
#include "DataStructures.h"

std::string get_file_contents(const char* filename);

class Shader
{
private:
	GLuint m_id;
	// 0 -> should be shaded
	// 1 -> custom data
	// 2 -> should have time input
	unsigned char m_flags = 0x00;
	std::unordered_map <std::string, UniformData> m_data;
public:

	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);
	void LoadShader(const char* vertexFile, const char* fragmentFile);
	void Bind();

	void SetInt(const char* uniformName, int value) const;
	void SetFloat(const char* uniformName, float value) const;
	void SetVec3(const char* uniformName, glm::vec3 vec) const;
	void SetMat4(const char* uniformName, glm::mat4& mat) const;

	void SetShading(bool isEnabled);
	bool GetShading();

	void SetCustomData(bool isEnabled);
	bool GetCustomData();
	void AddCustomData(std::string name, UniformData data);
	std::unordered_map <std::string, UniformData>& GetData();

	void SetTimeInput(bool isEnabled);
	bool GetTimeInput();
private:
	void CompileErrors(unsigned int shader, const char* type);
};