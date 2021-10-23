#pragma once
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "RenderCommand.h"

class Model;
class SceneNode
{
private:
	std::string m_name;
	Transform m_transform;
	std::vector<SceneNode*> m_children;
	std::vector<Component*> m_components;
	Model* m_model = nullptr;
public:
	SceneNode(std::string name);

	void Start();
	void Update(float deltaTime);
	void Render(Shader shader, glm::mat4 accumulatedTransform);
	void RenderSelf(Shader shader, glm::mat4 finalTranform);
	void MakeRenderCommand(std::vector<RenderCommand>& commands, glm::mat4 accumulatedTransform);

	void AppendChild(SceneNode* node);
	std::vector<SceneNode*> GetChildren();
	void AddComponent(Component* component);
	std::vector<Component*> GetComponents();
	Component* GetComponent(const char* className);
	std::string GetName();
	bool IsLeaf();

	Transform& GetTransform();
};

