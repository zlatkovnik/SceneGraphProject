#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"

class Light: public Component
{
protected:
	glm::vec3 m_color;
	float m_intensity;
public:
	Light();
	Light(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f);

	glm::vec3 GetColor();
	void SetColor(glm::vec3 color);
	float GetIntensity();
	void SetIntensity(float intensity);
};

