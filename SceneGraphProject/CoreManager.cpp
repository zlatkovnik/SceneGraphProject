#include "CoreManager.h"

#include "Debug.h"
#include "Camera.h"
#include "SceneNode.h"


void processInput(GLFWwindow* window)
{
    Camera* camera = CoreManager::GetInstance().GetMainCamera();
    float deltaTime = CoreManager::GetInstance().GetDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(Direction::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(Direction::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(Direction::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(Direction::RIGHT, deltaTime);
    // Ovo prebaciti da radi na prekid
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        CoreManager::GetInstance().ToggleCursor();
    }
}

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
    CoreManager::GetInstance().GetMainCamera()->ProcessMouseScroll(yoffset);
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

float CoreManager::GetDeltaTime()
{
    return m_deltaTime;
}

void CoreManager::ToggleCursor()
{
    if (!m_cursorEnabled) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    m_cursorEnabled = !m_cursorEnabled;
}

void CoreManager::HandleMouseMove(int newX, int newY)
{
    if (firstMouse)
    {
        m_lastX = newX;
        m_lastY = newY;
        firstMouse = false;
    }

    float xoffset = newX - m_lastX;
    float yoffset = m_lastY - newY;

    m_lastX = newX;
    m_lastY = newY;

    m_mainCamera->ProcessMouseMovement(xoffset, yoffset);
}

void CoreManager::SetRootNode(SceneNode* root)
{
    m_root = root;
}

void CoreManager::SetWindowSize(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void CoreManager::Init(int width, int height, std::string name)
{
	m_width = width;
	m_height = height;
    m_lastX = m_width / 2.0f;
    m_lastY = m_height / 2.0f;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_window = glfwCreateWindow(m_width, m_height, name.c_str(), NULL, NULL);
	if (m_window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGL();

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Debug::OpenglCallbackFunction, NULL);

    glEnable(GL_DEPTH_TEST);
}

void CoreManager::Run()
{
    auto standardShader = Shader::ShaderMap["standard"];
    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        processInput(m_window);

        m_root->Update(m_deltaTime);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        standardShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_mainCamera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(m_width / m_height), 0.1f, 100.0f);
        standardShader->SetMat4("view", view);
        standardShader->SetMat4("projection", projection);
        m_root->Render(*standardShader, glm::mat4(1.0f));
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CoreManager::Cleanup()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}