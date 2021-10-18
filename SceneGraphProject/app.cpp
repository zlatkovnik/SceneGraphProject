#include "CoreManager.h"
#include "Shader.h"
#include "Model.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "Rotator.h"
#include "LightRevolution.h"
#include "PointLight.h"
#include "Skybox.h"


int main() {

    CoreManager::GetInstance().Init(1920, 1080, "My Graph Scene Project");


    //Init resources
    Material standardMaterial;
    Material::MaterialLookup["standard"] = &standardMaterial;

	Shader standardShader("standard.vert", "standard.frag");
    Shader::ShaderLookup["standard"] = &standardShader;

    Shader skyboxShader("skybox.vert", "skybox.frag");
    Shader::ShaderLookup["skybox"] = &skyboxShader;

    std::vector<std::string> faces
    {
            "skybox/right.jpg",
            "skybox/left.jpg",
            "skybox/top.jpg",
            "skybox/bottom.jpg",
            "skybox/front.jpg",
            "skybox/back.jpg"
    };
    Skybox skybox(faces);
    CoreManager::GetInstance().SetSkybox(&skybox);

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


    SceneNode lightNode;
    lightNode.GetTransform().SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    PointLight pLight(&lightNode.GetTransform());
    LightRevolution revolution(&pLight);
    DirectionalLight dirLight(glm::vec3(-1.0f, -1.0f, -1.0f));
    lightNode.AddComponent(&dirLight);
    lightNode.AddComponent(&pLight);
    lightNode.AddComponent(&revolution);
    lightNode.AddComponent(backpack);
    root.AppendChild(&lightNode);

    root.Start();

    CoreManager::GetInstance().Run();
    CoreManager::GetInstance().Cleanup();
	return 0;
}