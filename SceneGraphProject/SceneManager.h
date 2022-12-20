#pragma once
#include <vector>
#include <nlohmann/json.hpp>

using namespace nlohmann;
class SceneManager
{
public:
	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	static SceneManager& GetInstance();

	void LoadScene(const char* path, const char* title = "No Title!", unsigned int screenWidth = 800, unsigned int screenHeight = 600, bool fullScreen = false);
	void RunScene();
private:
	SceneManager() {}
	void LoadSkybox(json scene);
	void LoadModels(json scene);
};

