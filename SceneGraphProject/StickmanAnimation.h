#pragma once
#include "Component.h"
#include "SceneNode.h"

class StickmanAnimation : public Component {
private:
	float m_acc = 0.0f;
	Shader* m_shader = nullptr;
	SceneNode* m_torsoNode = nullptr;
	SceneNode* m_leftArmNode = nullptr;
	SceneNode* m_rightArmNode = nullptr;
	SceneNode* m_leftLegNode = nullptr;
	SceneNode* m_rightLegNode = nullptr;
	SceneNode* m_headNode = nullptr;
public:
	StickmanAnimation(SceneNode* torsoNode) :m_torsoNode(torsoNode) { }

	void Start() {
		for (auto child : m_torsoNode->GetChildren()) {
			if (child->GetName().compare("leftArm") == 0) {
				m_leftArmNode = child;
			}
			if (child->GetName().compare("rightArm") == 0) {
				m_rightArmNode = child;
			}
			if (child->GetName().compare("leftLeg") == 0) {
				m_leftLegNode = child;
			}
			if (child->GetName().compare("rightLeg") == 0) {
				m_rightLegNode = child;
			}
			if (child->GetName().compare("head") == 0) {
				m_headNode = child;
			}
		}
		auto model = (Model*)m_torsoNode->GetComponent("class Model");
		m_shader = model->GetMaterial()->GetShader();
	}

	void Update(float deltaTime) {
		auto pi = glm::pi<float>();
		if (m_torsoNode) {
			float sinVal = 0.1 * glm::sin(4 * m_acc);
			m_torsoNode->GetTransform().SetRotation(0.0f, 0.0f, sinVal);
		}
		if (m_leftArmNode) {
			m_leftArmNode->GetTransform().SetRotation(0.8 * glm::sin(4 * -m_acc) + pi, 0.0f, -pi / 6);
		}
		if (m_rightArmNode) {
			m_rightArmNode->GetTransform().SetRotation(0.8 * -glm::sin(4 * -m_acc) + pi, 0.0f, pi / 6);
		}
		if (m_leftLegNode) {
			m_leftLegNode->GetTransform().SetRotation(0.8 * -glm::sin(4 * -m_acc) + pi, 0.0f, -pi / 12);
		}
		if (m_rightLegNode) {
			m_rightLegNode->GetTransform().SetRotation(0.8 * glm::sin(4 * -m_acc) + pi, 0.0f, pi / 12);
		}
		m_acc += deltaTime;
	}
};