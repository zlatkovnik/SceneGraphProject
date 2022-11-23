#pragma once
#include "Component.h"
#include "Transform.h"
class HarmonicTranslate : public Component {
private:
	Transform* m_transform;
	float m_speed = 1.0f, m_timeElapsed = 0.0f;
public:
	HarmonicTranslate(Transform* transform, float speed) {
		m_transform = transform;
		m_speed = speed;
	}
	virtual void Update(float deltaTime) {
		m_timeElapsed += deltaTime;
		m_transform->MoveTo(glm::vec3(0.0f, 0.0f, 3.0f * glm::sin(m_speed * m_timeElapsed)));
	}
};