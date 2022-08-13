#include "SceneManager.h"
#include "CoreManager.h"
#include "PointLight.h"
#include "LightRevolution.h"
#include "SceneNode.h"
#include "Model.h"

#include "Collider.h"
#include "Rotator.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


float fRandom() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


int main() {
	SceneManager::GetInstance().LoadScene("space.json");

	Shader* shader = &ResourceManager::GetInstance().ShaderLookup["standard"];
	Material* material = &Material::MaterialLookup["standard"];
	material->SetShader(shader);
	Model* rockModel = new Model("rock/rock.obj", material);

	for (int i = 0; i < 10000; i++) {
		SceneNode *rockNode = new SceneNode("rock" + std::to_string(i));
		rockModel->IncrementInstances();
		rockNode->AddComponent((Component*)rockModel);
		rockNode->GetTransform().MoveTo(glm::vec3(100.0f * fRandom() - 50.0f, 100.0f * fRandom() - 50.0f, 100.0f * fRandom() - 50.0f));
		CoreManager::GetInstance().GetRootNode()->AppendChild(rockNode);
	}

	SceneManager::GetInstance().RunScene();
	return 0;
}