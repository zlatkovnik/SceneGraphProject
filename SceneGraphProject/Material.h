#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <string>

class Shader;
class Material
{
private:
	std::string m_name;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
	Shader* m_shader;
public:
	static std::unordered_map<std::string, Material> MaterialLookup;
	Material();
	Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, Shader* shader);
	void SetMaterial();

	void SetName(std::string name);
	std::string GetName();
	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	void SetShininess(float shininess);
	void SetShader(Shader* shader);
	Shader* GetShader();
};

