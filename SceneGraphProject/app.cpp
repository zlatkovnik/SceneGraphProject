#include "SceneManager.h"
#include "CoreManager.h"
#include "PointLight.h"
#include "LightRevolution.h"
#include "SceneNode.h"
#include "Model.h"

#include "Collider.h"
#include "Rotator.h"
#include "Rotator2.h"
#include "Revolutor.h"
#include "SkeletalAnimation.h"
#include "HarmonicTranslate.h"
#include "ToggleHide.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



float fRandom() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void spaceConfig() {
	srand(glfwGetTime());
	Shader* shader = &ResourceManager::GetInstance().ShaderLookup["standard"];
	Material* material = &Material::MaterialLookup["standard"];
	material->SetShader(shader);
	Model* rockModel = new Model("rock/rock.obj", material);

	float radius = 50.0f;
	for (int i = 0; i < 126; i++) {
		SceneNode* rockNode = new SceneNode("rock" + std::to_string(i));
		rockModel->IncrementInstances();
		rockNode->AddComponent((Component*)rockModel);
		float randomAngle = 2 * glm::pi<float>() * fRandom();
		rockNode->GetTransform().MoveTo(glm::vec3(glm::sin(randomAngle) * radius, 20.0f * fRandom() - 5.0f, glm::cos(randomAngle) * radius));

		Rotator* rotator = new Rotator(&rockNode->GetTransform(), fRandom());
		rockNode->AddComponent(rotator);

		Revolutor* revolutor = new Revolutor(&rockNode->GetTransform(), 0.05f * fRandom() + 0.05f, 10.0f * fRandom() - 5.0f);
		rockNode->AddComponent(revolutor);

		CoreManager::GetInstance().GetRootNode()->AppendChild(rockNode);
	}

	Model* rockModel2 = new Model("rock2/rock2.obj", material);

	for (int i = 0; i < 126; i++) {
		SceneNode* rockNode = new SceneNode("rocki" + std::to_string(i));
		rockModel->IncrementInstances();
		rockNode->AddComponent((Component*)rockModel);
		float randomAngle = 2 * glm::pi<float>() * fRandom();
		rockNode->GetTransform().MoveTo(glm::vec3(glm::sin(randomAngle) * radius, 20.0f * fRandom() - 5.0f, glm::cos(randomAngle) * radius));

		Rotator* rotator = new Rotator(&rockNode->GetTransform(), fRandom());
		rockNode->AddComponent(rotator);

		Revolutor* revolutor = new Revolutor(&rockNode->GetTransform(), 0.05f * fRandom() + 0.05f, 10.0f * fRandom() - 5.0f);
		rockNode->AddComponent(revolutor);

		CoreManager::GetInstance().GetRootNode()->AppendChild(rockNode);
	}

	Model* rockModel3 = new Model("rock3/rock3.obj", material);

	for (int i = 0; i < 126; i++) {
		SceneNode* rockNode = new SceneNode("rockii" + std::to_string(i));
		rockModel->IncrementInstances();
		rockNode->AddComponent((Component*)rockModel);
		float randomAngle = 2 * glm::pi<float>() * fRandom();
		rockNode->GetTransform().MoveTo(glm::vec3(glm::sin(randomAngle) * radius, 20.0f * fRandom() - 5.0f, glm::cos(randomAngle) * radius));

		Rotator* rotator = new Rotator(&rockNode->GetTransform(), fRandom());
		rockNode->AddComponent(rotator);

		Revolutor* revolutor = new Revolutor(&rockNode->GetTransform(), 0.05f * fRandom() + 0.05f, 10.0f * fRandom() - 5.0f);
		rockNode->AddComponent(revolutor);

		CoreManager::GetInstance().GetRootNode()->AppendChild(rockNode);
	}
}

void lightsConfig() {
	//light_sphere
	SceneNode* lightSphere = CoreManager::GetInstance().GetNode("light_sphere", CoreManager::GetInstance().GetRootNode());
	PointLight* light = (PointLight*)lightSphere->GetComponent("class PointLight");
	Component* lightRevolution = new LightRevolution(light);
	lightSphere->AddComponent(lightRevolution);
}

void dungeonConfig() {
	SceneNode* propsContainer = CoreManager::GetInstance().GetNode("propsContainer", CoreManager::GetInstance().GetRootNode());
	Component* toggleHideProps = new ToggleHide(propsContainer, GLFW_KEY_R);
	propsContainer->AddComponent(toggleHideProps);

	SceneNode* wallsContainer = CoreManager::GetInstance().GetNode("wallsContainer", CoreManager::GetInstance().GetRootNode());
	Component* toggleHideWalls = new ToggleHide(wallsContainer, GLFW_KEY_Q);
	wallsContainer->AddComponent(toggleHideWalls);
}

void cratesConfig() {
	Shader* shader = &ResourceManager::GetInstance().ShaderLookup["standard"];
	Material* material = &Material::MaterialLookup["standard"];
	material->SetShader(shader);
	Model* crateModel = new Model("Crate/Crate1.obj", material);

	CoreManager::GetInstance().GetRootNode()->GetTransform().SetScale(glm::vec3(0.1f));
	SceneNode* prevNode = CoreManager::GetInstance().GetRootNode();
	for (int i = 0; i < 126; i++) {
		SceneNode* crate = new SceneNode("crate" + std::to_string(i));
		crateModel->IncrementInstances();
		crate->AddComponent((Component*)crateModel);

		crate->GetTransform().Translate(glm::vec3(0.0f, 2.0f, 0.0f));
		//crate->GetTransform().Rotate(glm::pi<float>() / 6, 0.0f, 0.0f);

		Rotator2* rotator = new Rotator2(&crate->GetTransform(), 0.01f, glm::vec3(1.0f, 0.0f, 1.0f));
		crate->AddComponent(rotator);

		prevNode->AppendChild(crate);
		prevNode = crate;
	}
}

void stickmanConfig() {
	CoreManager* coreManager = &CoreManager::GetInstance();
	SceneNode* torsoNode = coreManager->GetNode("torso", coreManager->GetRootNode());
	Component* stickmanAnimation = new SkeletalAnimation(torsoNode);
	torsoNode->AddComponent(stickmanAnimation);
}

void collisionConfig() {
	auto coreManager = &CoreManager::GetInstance();
	SceneNode* movingCollider = coreManager->GetNode("box", coreManager->GetRootNode());
	Component* harmonicTranslate = new HarmonicTranslate(&movingCollider->GetTransform(), 1.0f);
	movingCollider->AddComponent(harmonicTranslate);
}


int main() {
	SceneManager* sceneManager = &SceneManager::GetInstance();
	// Space
	//sceneManager->LoadScene("space.json", "Meteors!", 1440, 1080, true);
	//spaceConfig();

	// Lights
	sceneManager->LoadScene("light-scene.json", "Lights", 1440, 1080, true);
	lightsConfig();

	// Dungeon
	//sceneManager->LoadScene("dungeon.json", "DnD", 1440, 1080, true);
	//dungeonConfig();

	// Crates
	//sceneManager->LoadScene("crates.json", "Magic Crates", 1440, 1080, true);
	//cratesConfig();

	// Stickman
	//sceneManager->LoadScene("stickman.json", "Stickman", 1440, 1080, true);
	//stickmanConfig();

	// AABB
	//sceneManager->LoadScene("test.json", "AABB", 1440, 1080, true);
	//collisionConfig();

	sceneManager->RunScene();
	return 0;
}