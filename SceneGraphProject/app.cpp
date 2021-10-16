#include "CoreManager.h"
#include "Shader.h"
#include "Model.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Material.h"


int main() {

    CoreManager::GetInstance().Init(800, 600, "My Graph Scene Project");

	Shader basic("basic.vert", "basic.frag");
	basic.Bind();
    Shader::ShaderLookup["standard"] = &basic;

    Material standardMaterial;
    Material::MaterialLookup["standard"] = &standardMaterial;

    std::string path = "Crate/Crate1.obj";
    Model* backpack = new Model(path);

    SceneNode root;
    CoreManager::GetInstance().SetRootNode(&root);

    Camera camera;
    CoreManager::GetInstance().SetMainCamera(&camera);

    SceneNode crate;
    crate.AddComponent(backpack);
    root.AppendChild(&crate);

    SceneNode crate2;
    crate2.AddComponent(backpack);
    crate2.GetTransform().Translate(glm::vec3(0.0f, 3.0f, 0.0f));
    crate.AppendChild(&crate2);

    root.Start();

    CoreManager::GetInstance().Run();
    CoreManager::GetInstance().Cleanup();
	return 0;
}