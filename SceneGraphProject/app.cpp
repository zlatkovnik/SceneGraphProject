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


int main() {
	SceneManager::GetInstance().LoadScene("Test");
	auto node = CoreManager::GetInstance().GetNode("point-light", CoreManager::GetInstance().GetRootNode());
	PointLight* light = (PointLight*)node->GetComponent(typeid(PointLight).name());
	LightRevolution revolution(light);
	node->AddComponent(&revolution);

	// Test kutija
	//SceneNode box("test_box");
	//   Model* m = Model::CachedModels["Crate/Crate1.obj"];
	//   m->IncrementInstances();
	//   box.AddComponent(m);
	//box.GetTransform().MoveTo(glm::vec3(0.0f, -10.0f, 0.0f));
	//CoreManager::GetInstance().GetRootNode()->AppendChild(&box);

	// Wireframe
	//auto backpack = CoreManager::GetInstance().GetNode("backpack", CoreManager::GetInstance().GetRootNode());
	//Collider collider(glm::vec3(2.0f, 2.0f, 1.0f), glm::vec3(-2.0f, -2.0f, -1.0f), backpack);
	//backpack->AddComponent((Component*)&collider);
	//backpack->AddComponent(new Rotator(&backpack->GetTransform(), 1.0f));

	SceneManager::GetInstance().RunScene();
	return 0;
}