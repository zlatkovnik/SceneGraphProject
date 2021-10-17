#pragma once
#include "PointLight.h"

class LightRevolution: public Component {
private:
	PointLight* m_light;
	float m_acc = 0.0f;
public:
	LightRevolution(PointLight* light) {
		m_light = light;
	}
	void Update(float deltaTime) override {
		m_acc += deltaTime;
		auto transform = m_light->GetTransform();
		transform->MoveTo(glm::vec3(cos(m_acc) * 2.0f, 0.0f, sin(m_acc) * 2.0f));
	}
};