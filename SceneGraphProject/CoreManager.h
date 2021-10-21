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
class Skybox;
class EventObserver;
class CoreManager
{
private:
    SceneNode* m_root;
    Camera* m_mainCamera;
    GLFWwindow* m_window;
    int m_width, m_height;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;

    std::vector<EventObserver*> m_observers;
public:
    void Init(int width, int height, std::string name);
    void Run();
    void Cleanup();

    CoreManager(CoreManager const&) = delete;
    void operator=(CoreManager const&) = delete;

    static CoreManager& GetInstance();
    SceneNode* GetRootNode();
    void SetRootNode(SceneNode* root);
    SceneNode* GetNode(std::string name, SceneNode* root);
    Camera* GetMainCamera();
    void SetMainCamera(Camera* camera);
    float GetDeltaTime();
    void GetWindowSize(int* width, int* height);
    void SetWindowSize(int width, int height);
    void HandleMouseMove(double newX, double newY);
    void HandleScroll(double newY);
    void HandleInput(GLFWwindow* window);
    void HandleInputPress(int key, int action);

    void EnableCursor();
    void DisableCursor();

    void RegisterEventObserver(EventObserver* observer);
private:
    CoreManager(): m_root(nullptr) {}

};

