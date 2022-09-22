#pragma once
#include "Component.h"
#include "Transform.h"
class Rotator2: public Component {
private:
	Transform* m_transform;
	float m_speed = 1.0f;
	glm::vec3 m_axis;
public:
	Rotator2(Transform* transform, float speed, glm::vec3 axis)
	:m_transform(transform), m_speed(speed), m_axis(axis) { }
	virtual void Update(float deltaTime) {
		auto dAngle = m_speed * deltaTime;
		if (m_axis[0] > 0.0f) {
			m_transform->Rotate(dAngle, 0.0f, 0.0f);
		}
		if (m_axis[1] > 0.0f) {
			m_transform->Rotate(0.0f, dAngle, 0.0f);
		}
		if (m_axis[2] > 0.0f) {
			m_transform->Rotate(0.0f, 0.0f, dAngle);
		}
	}
};