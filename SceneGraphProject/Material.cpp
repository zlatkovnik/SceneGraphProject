#include "Material.h"
#include "Shader.h"

std::unordered_map<std::string, Material> Material::MaterialLookup;

Material::Material()
	:m_name("default"), m_ambient(glm::vec3(0.1f, 0.1f, 0.1f)), m_diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_specular(glm::vec3(0.5f, 0.5f, 0.5f)), m_shininess(32.0f), m_shader(nullptr)
{
}

Material::Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, Shader* shader)
	:m_name(name), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_shader(shader)
{
}

void Material::SetMaterial()
{
	m_shader->SetVec3("material.ambient", m_ambient);
	m_shader->SetVec3("material.diffuse", m_diffuse);
	m_shader->SetVec3("material.specular", m_specular);
	m_shader->SetFloat("material.shininess", m_shininess);
}

void Material::SetName(std::string name)
{
	m_name = name;
}

std::string Material::GetName()
{
	return m_name;
}

void Material::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void Material::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void Material::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}

void Material::SetShininess(float shininess)
{
	m_shininess = shininess;
}

void Material::SetShader(Shader* shader)
{
	m_shader = shader;
}

Shader* Material::GetShader()
{
	return m_shader;
}
