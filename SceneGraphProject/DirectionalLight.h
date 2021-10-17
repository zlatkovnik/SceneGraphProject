#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalLight: public Light
{
private:
	glm::vec3 m_direction;
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f);

	glm::vec3 GetDirection();
	void SetDirection(glm::vec3 direction);
};

