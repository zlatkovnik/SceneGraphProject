#pragma once
#include <vector>
#include "SceneNode.h"
#include "Skybox.h"
#include "Model.h"
#include <nlohmann/json.hpp>

using namespace nlohmann;
class SceneManager
{
public:
	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	static SceneManager& GetInstance();

	void LoadScene(const char* path);
private:
	SceneManager() {}
	void LoadSkybox(json scene, Skybox& skybox);
	void LoadModels(json scene);
};

