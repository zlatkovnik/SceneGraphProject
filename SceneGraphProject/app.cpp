#include "SceneManager.h"
#include "CoreManager.h"
#include "PointLight.h"
#include "LightRevolution.h"
#include "SceneNode.h"
#include "Model.h"

#include "Collider.h"
#include "Rotator.h"
#include "Revolutor.h"

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
		rockNode->GetTransform().MoveTo(glm::vec3(glm::sin(randomAngle) * radius, 10.0f * fRandom() - 5.0f, glm::cos(randomAngle) * radius));

		Rotator* rotator = new Rotator(&rockNode->GetTransform(), fRandom());
		rockNode->AddComponent(rotator);

		Revolutor* revolutor = new Revolutor(&rockNode->GetTransform(), 0.05f * fRandom() + 0.05f, 10.0f * fRandom() - 5.0f);
		rockNode->AddComponent(revolutor);

		CoreManager::GetInstance().GetRootNode()->AppendChild(rockNode);
	}
}


int main() {
	SceneManager* sceneManager = &SceneManager::GetInstance();
	// Space
	sceneManager->LoadScene("space.json");
	spaceConfig();

	// Dungeon
	//sceneManager->LoadScene("dungeon.json");

	sceneManager->RunScene();
	return 0;
}