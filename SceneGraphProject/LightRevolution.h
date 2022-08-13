#pragma once
#include "PointLight.h"
#include "EventObserver.h"

class LightRevolution: public Component, public EventObserver {
private:
	PointLight* m_light;
	float m_acc = 0.0f;
	bool m_moving = true;
public:
	LightRevolution(PointLight* light) {
		m_light = light;
	}
	void Start() override {
		CoreManager::GetInstance().RegisterEventObserver(this);
	}
	void Update(float deltaTime) override {
		if (m_moving) {
			m_acc += deltaTime;
			auto transform = m_light->GetTransform();
			transform->MoveTo(glm::vec3(cos(m_acc) * 2.0f, 0.0f, sin(m_acc) * 2.0f));
		}
	}

	void KeyPress(int key, int action) override {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			m_moving = !m_moving;
		}
	};
};