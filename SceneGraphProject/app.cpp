#include "SceneManager.h"
#include "CoreManager.h"
#include "PointLight.h"
#include "LightRevolution.h"


int main() {
	SceneManager::GetInstance().LoadScene("Test");
	auto node = CoreManager::GetInstance().GetNode("point-light", CoreManager::GetInstance().GetRootNode());
	PointLight* light = (PointLight*)node->GetComponent(typeid(PointLight).name());
	LightRevolution revolution(light);
	node->AddComponent(&revolution);
	SceneManager::GetInstance().RunScene();
	return 0;
}