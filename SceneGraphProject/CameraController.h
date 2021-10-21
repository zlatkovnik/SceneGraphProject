#pragma once
#include "Component.h"
#include "Camera.h"
#include "EventObserver.h"
#include "CoreManager.h"

class CameraController : public Component, public EventObserver {
private:
	Camera* m_camera;
	float m_lastX;
	float m_lastY;
	bool m_firstMouse = true;
	bool m_cursorEnabled = false;
public:
	CameraController(Camera* camera) {
		m_camera = camera;
	}
	void Start() override {
		CoreManager::GetInstance().RegisterEventObserver(this);
	}
	void Update(float deltaTime) override {

	}

	void MouseMove(double newX, double newY) override {
		if (m_firstMouse)
		{
			m_lastX = newX;
			m_lastY = newY;
			m_firstMouse = false;
		}

		float xoffset = newX - m_lastX;
		float yoffset = m_lastY - newY;

		m_lastX = newX;
		m_lastY = newY;

		m_camera->ProcessMouseMovement(xoffset, yoffset);
	}

	void KeyDown(GLFWwindow* window) override {
		float deltaTime = CoreManager::GetInstance().GetDeltaTime();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_camera->ProcessKeyboard(Direction::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_camera->ProcessKeyboard(Direction::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_camera->ProcessKeyboard(Direction::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_camera->ProcessKeyboard(Direction::RIGHT, deltaTime);
	}

	void Scroll(double newY) override {
		m_camera->ProcessMouseScroll(newY);
	}

	void KeyPress(int key, int action) {
		if (key == GLFW_KEY_E && action == GLFW_PRESS) {
			if (m_cursorEnabled) {
				CoreManager::GetInstance().DisableCursor();
			}
			else {
				CoreManager::GetInstance().EnableCursor();
			}
			m_cursorEnabled = !m_cursorEnabled;
		}
	}
};