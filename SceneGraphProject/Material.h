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
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
public:
	static std::unordered_map<std::string, Material> MaterialLookup;
	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	void SetMaterial(Shader* shader);

	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	void SetShininess(float shininess);
};

