#pragma once
#include "Transform.h"
#include "Light.h"
class PointLight: public Light
{
private:
	Transform* m_transform;
	float m_range;
public:
	PointLight(Transform* transform, glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f, float range = 1.0f);

	Transform* GetTransform();
	void SetTransform(Transform* transform);
	float GetRange();
	void SetRange(float range);
};

