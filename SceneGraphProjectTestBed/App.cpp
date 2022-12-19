#include "../SceneGraphProject/CoreManager.h"
#include "../SceneGraphProject/CoreManager.cpp"
#include "../SceneGraphProject/SceneManager.h"
#include "../SceneGraphProject/SceneManager.cpp"
#include "../SceneGraphProject/Libraries/include/glad/glad.h"
#include "../SceneGraphProject/Libraries/include/glm/glm.hpp"

int main() {
	SceneManager* sceneManager = &SceneManager::GetInstance();
	sceneManager->LoadScene("dungeon.json", "DnD", 1440, 1080);

	// Crates
	//sceneManager->LoadScene("crates.json", "Magic Crates", 1440, 1080);
	//cratesConfig();

	// Stickman
	//sceneManager->LoadScene("stickman.json", "Stickman", 1440, 1080);
	//stickmanConfig();

	// AABB
	//sceneManager->LoadScene("test.json", "AABB", 1440, 1080);
	//collisionConfig();

	sceneManager->RunScene();
	return 0;
}