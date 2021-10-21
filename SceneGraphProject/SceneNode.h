#pragma once
#include <vector>
#include "Transform.h"
#include "Component.h"

class SceneNode
{
private:
	std::string m_name;
	Transform m_transform;
	std::vector<SceneNode*> m_children;
	std::vector<Component*> m_components;
public:
	SceneNode(std::string name);

	void Start();
	void Update(float deltaTime);
	void Render(Shader shader, glm::mat4 accumulatedTransform);

	void AppendChild(SceneNode* node);
	void AddComponent(Component* component);
	bool IsLeaf();

	Transform& GetTransform();
};

