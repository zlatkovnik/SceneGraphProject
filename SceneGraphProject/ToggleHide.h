#pragma once
#include "Component.h"
#include "SceneNode.h"
#include "EventObserver.h"
#include <GLFW/glfw3.h>

class ToggleHide : public Component, public EventObserver {
private:
	SceneNode* m_sceneNodeGroup;
	SceneNode* m_parentNode;
	bool m_hidden = false;
	unsigned int m_key;
public:
	ToggleHide(SceneNode* sceneNodeGroup, unsigned int key): m_sceneNodeGroup(sceneNodeGroup), m_key(key) {
		m_parentNode = sceneNodeGroup->GetParentNode();
	}
	void Start() override {
		CoreManager::GetInstance().RegisterEventObserver(this);
	}
	void KeyPress(int key, int action) override {
		if (key == m_key && action == GLFW_PRESS) {
			if (m_hidden) {
				m_parentNode->AppendChild(m_sceneNodeGroup);
			}
			else {
				m_parentNode->DetachChild(m_sceneNodeGroup);
			}
			m_hidden = !m_hidden;
		}
	}
};