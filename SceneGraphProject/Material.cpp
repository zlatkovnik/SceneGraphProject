#include "Material.h"
#include "Shader.h"

std::unordered_map<std::string, Material> Material::MaterialLookup;

Material::Material()
	:m_ambient(glm::vec3(0.1f, 0.1f, 0.1f)), m_diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_specular(glm::vec3(0.5f, 0.5f, 0.5f)), m_shininess(32.0f)
{
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	:m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{
}

void Material::SetMaterial(Shader* shader)
{
	shader->SetVec3("material.ambient", m_ambient);
	shader->SetVec3("material.diffuse", m_diffuse);
	shader->SetVec3("material.specular", m_specular);
	shader->SetFloat("material.shininess", m_shininess);
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
