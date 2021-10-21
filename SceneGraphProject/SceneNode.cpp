#include "SceneNode.h"
#include <typeinfo> 
#include "DirectionalLight.h"
#include "PointLight.h"
#include "CoreManager.h"
#include "Model.h"
#include "RenderManager.h"

SceneNode::SceneNode(std::string name)
    :m_name(name)
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

std::vector<SceneNode*> SceneNode::GetChildren()
{
    return m_children;
}

void SceneNode::AddComponent(Component* component)
{
    if (typeid(*component) == typeid(Model) || 
        typeid(*component) == typeid(DirectionalLight) || 
        typeid(*component) == typeid(PointLight)) {
        RenderManager::GetInstance().QueueRender(component);
    }
    m_components.push_back(component);
}

std::vector<Component*> SceneNode::GetComponents()
{
    return m_components;
}

Component* SceneNode::GetComponent(const char* className)
{
    for (int i = 0; i < m_components.size(); i++) {
        auto name = std::string(typeid(*m_components[i]).name());
        if (name.compare(className) == 0) {
            return m_components[i];
        }
    }
    return nullptr;
}

std::string SceneNode::GetName()
{
    return m_name;
}

bool SceneNode::IsLeaf()
{
    return m_children.size() == 0;
}

Transform& SceneNode::GetTransform()
{
    return m_transform;
}
