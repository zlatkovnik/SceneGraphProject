#pragma once
#include "Component.h"
#include "Transform.h"
class Rotator : public Component {
private:
	Transform* m_transform;
	float m_speed = 1.0f, m_angle = 0.0f;
public:
	Rotator(Transform* transform, float speed) {
		m_transform = transform;
		m_speed = speed;
	}
	virtual void Update(float deltaTime) {
		m_angle += deltaTime * m_speed;
		m_transform->SetRotation(0.0f, 0.0f, m_angle);
	}
};