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
    for (int i = 0; i < m_components.size(); i++) {
        m_components[i]->Start();
    }
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->Start();
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
    if (m_model != nullptr) {
        m_model->Render();
    }
    for (auto child : m_children) {
        child->Render(shader, currentTransform);
    }
}

void SceneNode::RenderSelf(Shader shader, glm::mat4 finalTranform)
{
    if (m_model != nullptr) {
        shader.SetMat4("model", finalTranform);
        m_model->Render();
    }
}

void SceneNode::MakeRenderCommand(std::vector<RenderCommand>& commands, glm::mat4 accumulatedTransform)
{
    glm::mat4 currentTransform = accumulatedTransform * m_transform.GetTransformMatrix();
    if (m_model != nullptr) {
        RenderCommand cmd(m_model, m_model->GetMaterial()->GetShader(), currentTransform);
        commands.push_back(cmd);
    }
    for (auto &child : m_children) {
        child->MakeRenderCommand(commands, currentTransform);
    }
}

void SceneNode::AppendChild(SceneNode* node)
{
    node->SetParentNode(this);
    m_children.push_back(node);
}

std::vector<SceneNode*> SceneNode::GetChildren()
{
    return m_children;
}

void SceneNode::DetachChild(SceneNode* child)
{
    auto it = std::remove_if(m_children.begin(), m_children.end(), [=](const SceneNode* sn) {
        return sn->GetName() == child->GetName();
        });
    m_children.erase(it, it + 1);
}

void SceneNode::AddComponent(Component* component)
{
    if (typeid(*component) == typeid(Model) || 
        typeid(*component) == typeid(DirectionalLight) || 
        typeid(*component) == typeid(PointLight)) {
        RenderManager::GetInstance().QueueRender(component);
    }
    if (typeid(*component) == typeid(Model)) {
        m_model = (Model*)component;
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

std::string SceneNode::GetName() const
{
    return m_name;
}

bool SceneNode::IsLeaf()
{
    return m_children.size() == 0;
}

void SceneNode::SetParentNode(SceneNode* parentNode)
{
    m_parentNode = parentNode;
}

SceneNode* SceneNode::GetParentNode()
{
    return m_parentNode;
}

Transform& SceneNode::GetTransform()
{
    return m_transform;
}
