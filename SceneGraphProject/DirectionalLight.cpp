#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:Light(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), m_direction(glm::vec3(-1.0f, -1.0f, -1.0f))
{
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
	: Light(color, intensity), m_direction(direction)
{
}

glm::vec3 DirectionalLight::GetDirection()
{
	return m_direction;
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}
