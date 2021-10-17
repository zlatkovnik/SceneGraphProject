#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneNode;
class Camera;
class Shader;
class DirectionalLight;
class PointLight;
class Light;
class CoreManager
{
private:
    SceneNode* m_root;
    Camera* m_mainCamera;
    GLFWwindow* m_window;
    glm::mat4 m_projection;
    int m_width, m_height;
    bool m_cursorEnabled = false;
    float m_lastX;
    float m_lastY;
    bool firstMouse = true;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    // Prebaciti kasnije
    std::vector<DirectionalLight*> m_directionalLights;
    std::vector<PointLight*> m_pointLights;
public:
    void Init(int width, int height, std::string name);
    void Run();
    void Cleanup();

    CoreManager(CoreManager const&) = delete;
    void operator=(CoreManager const&) = delete;

    static CoreManager& GetInstance();
    SceneNode* GetRootNode();
    void SetRootNode(SceneNode* root);
    Camera* GetMainCamera();
    void SetMainCamera(Camera* camera);
    void UpdateProjection(Shader* shader);
    float GetDeltaTime();
    void ToggleCursor();
    void HandleMouseMove(int newX, int newY);
    void SetWindowSize(int width, int height);

    void AddLight(Light* light);
private:
    CoreManager(): m_root(nullptr) {}
};

