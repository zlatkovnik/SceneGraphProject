#include "SceneNode.h"

SceneNode::SceneNode()
{
}

SceneNode::SceneNode(Transform transform)
    :m_transform(transform)
{
}

void SceneNode::Start()
{
    for (auto component : m_components) {
        component.Start();
    }
    for (auto child : m_children) {
        child->Start();
    }
}

void SceneNode::Update(float deltaTime)
{
    for (auto component : m_components) {
        component.Update(deltaTime);
    }
    for (auto child : m_children) {
        child->Update(deltaTime);
    }
}

void SceneNode::Render()
{
    for (auto component : m_components) {
        component.Render();
    }
    for (auto child : m_children) {
        child->Render();
    }
}

bool SceneNode::IsLeaf()
{
    return m_children.size() == 0;
}
