#pragma once
#include <vector>
#include "Transform.h"
#include "Component.h"

class SceneNode
{
private:
	Transform m_transform;
	std::vector<SceneNode*> m_children;
	std::vector<Component> m_components;
public:
	SceneNode();
	SceneNode(Transform transform);

	void Start();
	void Update(float deltaTime);
	void Render();


	bool IsLeaf();
};

