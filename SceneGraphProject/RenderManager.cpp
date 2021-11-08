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
#include "ResourceManager.h"

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
    // Instanced rendering
    static std::vector<RenderCommand> commands(1024);
    commands.clear();
    root->MakeRenderCommand(commands, glm::mat4(1.0f));
    std::unordered_map<Shader*, std::unordered_map<Model*, std::vector<RenderCommand>>> groupedCommands;
    for (int i = 0; i < commands.size(); i++) {
        groupedCommands[commands[i].m_model->GetMaterial()->GetShader()][commands[i].m_model].push_back(commands[i]);
    }
    for (auto& commandByShader : groupedCommands) {
        commandByShader.first->Bind();
        if (commandByShader.first->GetShading()) {
            SetLightUniforms(commandByShader.first);
        }
        if (commandByShader.first->GetCustomData()) {
            for (auto data : commandByShader.first->GetData()) {
                if (data.second.m_type == "vec3") {
                    glm::vec3 *vec = (glm::vec3*)data.second.m_value;
                    commandByShader.first->SetVec3(data.first.c_str(), *vec);
                }
            }
        }
        if (commandByShader.first->GetTimeInput()) {
            commandByShader.first->SetFloat("time", (float)glfwGetTime());
        }
        for (auto& command : commandByShader.second) {
            for (int i = 0; i < command.second.size(); i++) {
                glm::mat4 mvp = projection * view * command.second[i].m_transformMatrix;
                char buff[128];
                sprintf_s(buff, "mvps[%d]", i);
                command.second[i].m_shader->SetMat4(buff, mvp);
                sprintf_s(buff, "models[%d]", i);
                command.second[i].m_shader->SetMat4(buff, command.second[i].m_transformMatrix);
            }
            command.first->Render();
        }
    }
    // Old deprecated
    //root->Render(*m_standardShader, glm::mat4(1.0f));
    //Skybox
    if (m_skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        auto& skyboxShader = ResourceManager::GetInstance().ShaderLookup["skybox"];
        skyboxShader.Bind();
        view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        skyboxShader.SetMat4("view", view);
        skyboxShader.SetMat4("projection", projection);
        skyboxShader.SetInt("skybox", 0);
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
}

void RenderManager::SetLightUniforms(Shader* shader)
{
    Camera* camera = CoreManager::GetInstance().GetMainCamera();
    shader->SetInt("directionalLightsCount", m_directionalLights.size());
    shader->SetVec3("viewPos", camera->GetPosition());
    for (int i = 0; i < m_directionalLights.size(); i++) {
        char buff[128];
        sprintf_s(buff, "directionalLights[%d].direction", i);
        shader->SetVec3(buff, m_directionalLights[i]->GetDirection());
        sprintf_s(buff, "directionalLights[%d].color", i);
        shader->SetVec3(buff, m_directionalLights[i]->GetColor());
        sprintf_s(buff, "directionalLights[%d].intensity", i);
        shader->SetFloat(buff, m_directionalLights[i]->GetIntensity());
    }
    shader->SetInt("pointLightsCount", m_pointLights.size());
    for (int i = 0; i < m_pointLights.size(); i++) {
        char buff[128];
        sprintf_s(buff, "pointLights[%d].position", i);
        shader->SetVec3(buff, m_pointLights[i]->GetTransform()->GetPosition());
        sprintf_s(buff, "pointLights[%d].color", i);
        shader->SetVec3(buff, m_pointLights[i]->GetColor());
        sprintf_s(buff, "pointLights[%d].intensity", i);
        shader->SetFloat(buff, m_pointLights[i]->GetIntensity());
    }
}
