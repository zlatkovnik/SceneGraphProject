#include "RenderManager.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <functional>
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SceneNode.h"
#include "Skybox.h"
#include "Component.h"
#include "Model.h"
#include "CoreManager.h"

RenderManager& RenderManager::GetInstance()
{
    static RenderManager instance;
    return instance;
}

void RenderManager::RenderAll(SceneNode* root)
{
    Camera* camera = CoreManager::GetInstance().GetMainCamera();
    int width, height;
    CoreManager::GetInstance().GetWindowSize(&width, &height);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)(width / height), 0.1f, 100.0f);
    m_standardShader->Bind();
    // Lights
    m_standardShader->SetInt("directionalLightsCount", m_directionalLights.size());
    m_standardShader->SetVec3("viewPos", camera->GetPosition());
    for (int i = 0; i < m_directionalLights.size(); i++) {
        char buff[128];
        sprintf_s(buff, "directionalLights[%d].direction", i);
        m_standardShader->SetVec3(buff, m_directionalLights[i]->GetDirection());
        sprintf_s(buff, "directionalLights[%d].color", i);
        m_standardShader->SetVec3(buff, m_directionalLights[i]->GetColor());
        sprintf_s(buff, "directionalLights[%d].intensity", i);
        m_standardShader->SetFloat(buff, m_directionalLights[i]->GetIntensity());
    }
    m_standardShader->SetInt("pointLightsCount", m_pointLights.size());
    for (int i = 0; i < m_pointLights.size(); i++) {
        char buff[128];
        sprintf_s(buff, "pointLights[%d].position", i);
        m_standardShader->SetVec3(buff, m_pointLights[i]->GetTransform()->GetPosition());
        sprintf_s(buff, "pointLights[%d].color", i);
        m_standardShader->SetVec3(buff, m_pointLights[i]->GetColor());
        sprintf_s(buff, "pointLights[%d].intensity", i);
        m_standardShader->SetFloat(buff, m_pointLights[i]->GetIntensity());
    }
    static std::vector<RenderCommand> commands;
    commands.clear();
    root->MakeRenderCommand(commands, glm::mat4(1.0f));
    for (auto command : commands) {
        // TODO OPTIMIZE
        glm::mat4 identity = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * command.m_transformMatrix;
        m_standardShader->SetMat4("mvp", mvp);
        m_standardShader->SetMat4("model", command.m_transformMatrix);
        command.m_model->Render(*m_standardShader);
    }
    // Old deprecated
    //root->Render(*m_standardShader, glm::mat4(1.0f));
    //Skybox
    if (m_skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        m_skyboxShader->Bind();
        view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        m_skyboxShader->SetMat4("view", view);
        m_skyboxShader->SetMat4("projection", projection);
        m_skyboxShader->SetInt("skybox", 0);
        m_skybox->Draw();
        glDepthFunc(GL_LESS);
    }
}

void RenderManager::SetSkybox(Skybox* skybox)
{
    m_skybox = skybox;
}

void RenderManager::QueueRender(Component* component)
{
    auto type = typeid(*component).name();
    if (type == typeid(Model).name()) {
        m_models.push_back((Model*)component);
    }
    else if (type == typeid(PointLight).name()) {
        m_pointLights.push_back((PointLight*)component);
    }
    else if (type == typeid(DirectionalLight).name()) {
        m_directionalLights.push_back((DirectionalLight*)component);
    }
    else {
        throw "[RenderManager] Component not registered!";
    }
}

RenderManager::RenderManager() {
    m_standardShader = &Shader::ShaderLookup["standard"];
    m_skyboxShader = &Shader::ShaderLookup["skybox"];
}