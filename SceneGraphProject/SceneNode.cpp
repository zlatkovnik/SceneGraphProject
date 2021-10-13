#include "SceneNode.h"

SceneNode::SceneNode()
{
}

void SceneNode::Start()
{
    for (auto component : m_components) {
        component->Start();
    }
    for (auto child : m_children) {
        child->Start();
    }
}

void SceneNode::Update(float deltaTime)
{
    for (auto component : m_components) {
        component->Update(deltaTime);
    }
    for (auto child : m_children) {
        child->Update(deltaTime);
    }
}

void SceneNode::Render(Shader shader, glm::mat4 accumulatedTransform)
{
    glm::mat4 currentTransform = accumulatedTransform * m_transform.GetTransformMatrix();
    shader.SetMat4("model", currentTransform);
    for (auto component : m_components) {
        component->Render(shader);
    }
    for (auto child : m_children) {
        child->Render(shader, currentTransform);
    }
}

void SceneNode::AppendChild(SceneNode* node)
{
    m_children.push_back(node);
}

void SceneNode::AddComponent(Component* component)
{
    m_components.push_back(component);
}

bool SceneNode::IsLeaf()
{
    return m_children.size() == 0;
}

Transform& SceneNode::GetTransform()
{
    return m_transform;
}
