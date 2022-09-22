#pragma once
#include "Component.h"
#include "Transform.h"

class Revolutor : public Component {
private:
	Transform* m_transform;
	float m_speed = 0.1f;
	float m_yOffset = 0.0f;

public:
	Revolutor(Transform* transform, float speed, float yOffset)
		:m_speed(speed), m_yOffset(yOffset) {
		m_transform = transform;
	}
	virtual void Update(float deltaTime) {
		float x = m_transform->GetPosition().x, y = m_transform->GetPosition().z;
		float mag = glm::sqrt(x * x + y * y);
		float currAngle = glm::atan(y / mag, x / mag);
		currAngle += deltaTime * m_speed;
		float newX = glm::cos(currAngle) * mag, newY = glm::sin(currAngle) * mag;
		m_transform->MoveTo(glm::vec3(newX, m_yOffset, newY));
	}
};