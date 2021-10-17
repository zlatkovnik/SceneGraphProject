#include "Light.h"

Light::Light()
	:Component(), m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_intensity(1.0f)
{
}

Light::Light(glm::vec3 color, float intensity)
	: Component(), m_color(color), m_intensity(intensity)
{
}

glm::vec3 Light::GetColor()
{
	return m_color;
}

void Light::SetColor(glm::vec3 color)
{
	m_color = color;
}

float Light::GetIntensity()
{
	return m_intensity;
}

void Light::SetIntensity(float intensity)
{
	m_intensity = intensity;
}
