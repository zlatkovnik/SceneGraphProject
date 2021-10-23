#pragma once
#define CMD_BUFF_SIZE 1024
#include <vector>
#include "Transform.h"
#include "RenderCommand.h"

class Component;
class Shader;
class DirectionalLight;
class PointLight;
class Camera;
class Skybox;
class SceneNode;
class Model;

class RenderManager
{
private:
	std::vector<DirectionalLight*> m_directionalLights;
	std::vector<PointLight*> m_pointLights;
	std::vector<Model*> m_models;
	Skybox* m_skybox = nullptr;

	// Cache
	Shader* m_standardShader = nullptr;
	Shader* m_skyboxShader = nullptr;

public:
	static RenderManager& GetInstance();
	RenderManager(RenderManager const&) = delete;
	void operator=(RenderManager const&) = delete;

	void QueueRender(Component* component);
	void RenderAll(SceneNode* root);

	void SetSkybox(Skybox* skybox);
private:
	RenderManager();
};

