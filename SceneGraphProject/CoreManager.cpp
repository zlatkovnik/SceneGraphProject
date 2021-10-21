#include "CoreManager.h"
#include "Debug.h"
#include "Camera.h"
#include "SceneNode.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "RenderManager.h"
#include "EventObserver.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    CoreManager::GetInstance().SetWindowSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    CoreManager::GetInstance().HandleMouseMove(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CoreManager::GetInstance().HandleScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CoreManager::GetInstance().HandleInputPress(key, action);
}


CoreManager& CoreManager::GetInstance() {
    static CoreManager instance;
    return instance;
}

SceneNode* CoreManager::GetRootNode()
{
    return m_root;
}

Camera* CoreManager::GetMainCamera()
{
    return m_mainCamera;
}

void CoreManager::SetMainCamera(Camera* camera)
{
    m_mainCamera = camera;
}

void CoreManager::RegisterEventObserver(EventObserver* observer)
{
    m_observers.push_back(observer);
}

void CoreManager::GetWindowSize(int* width, int* height)
{
    *width = m_width;
    *height = m_height;
}

void CoreManager::SetWindowSize(int width, int height)
{
    m_width = width; m_height = height;
    glViewport(0, 0, m_width, m_height);
}

float CoreManager::GetDeltaTime()
{
    return m_deltaTime;
}

void CoreManager::HandleMouseMove(double newX, double newY)
{
    for (int i = 0; i < m_observers.size(); i++) {
        m_observers[i]->MouseMove(newX, newY);
    }
}

void CoreManager::HandleScroll(double newY)
{
    for (int i = 0; i < m_observers.size(); i++) {
        m_observers[i]->Scroll(newY);
    }
}

void CoreManager::HandleInput(GLFWwindow* window)
{
    for (int i = 0; i < m_observers.size(); i++) {
        m_observers[i]->KeyDown(window);
    }
}

void CoreManager::HandleInputPress(int key, int action)
{
    for (int i = 0; i < m_observers.size(); i++) {
        m_observers[i]->KeyPress(key, action);
    }
}

void CoreManager::EnableCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CoreManager::DisableCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CoreManager::SetRootNode(SceneNode* root)
{
    m_root = root;
}

void CoreManager::Init(int width, int height, std::string name)
{
    m_width = width;
    m_height = height;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (m_window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetKeyCallback(m_window, key_callback);

    DisableCursor();

	gladLoadGL();

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Debug::OpenglCallbackFunction, NULL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void CoreManager::Run()
{
    auto standardShader = Shader::ShaderLookup["standard"];
    auto skyboxShader = Shader::ShaderLookup["skybox"];
    m_root->Start();
    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        HandleInput(m_window);

        m_root->Update(m_deltaTime);
        
        RenderManager::GetInstance().RenderAll(m_root);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CoreManager::Cleanup()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}