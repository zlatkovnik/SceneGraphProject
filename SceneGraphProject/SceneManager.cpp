#include "SceneManager.h"
#include "CoreManager.h"
#include "RenderManager.h"
#include "Material.h"
#include "Skybox.h"
#include "Camera.h"
#include "CameraController.h"
#include "LightRevolution.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Transform.h"
#include "SceneNode.h"
#include "Model.h"

SceneNode* LoadChild(json root);

SceneManager& SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::LoadScene(const char* path)
{
    CoreManager::GetInstance().Init(1920, 1080, "My Graph Scene Project");

    // Ovo prebaciti u poseban menadzer
    Material standardMaterial;
    Material::MaterialLookup["standard"] = standardMaterial;

    Shader standardShader("standard.vert", "standard.frag");
    Shader::ShaderLookup["standard"] = standardShader;

    Shader skyboxShader("skybox.vert", "skybox.frag");
    Shader::ShaderLookup["skybox"] = skyboxShader;

    std::ifstream stream("test.json");
    json scene;
    stream >> scene;

    LoadSkybox(scene);

    LoadModels(scene);

    SceneNode* cameraNode = new SceneNode("camera");
    Camera* camera = new Camera();
    CameraController* cameraController = new CameraController(camera);
    cameraNode->AddComponent(camera);
    cameraNode->AddComponent(cameraController);
    CoreManager::GetInstance().GetRootNode()->AppendChild(cameraNode);
    CoreManager::GetInstance().SetMainCamera(camera);
}

void SceneManager::RunScene()
{
    CoreManager::GetInstance().Run();
    CoreManager::GetInstance().Cleanup();
}

void SceneManager::LoadSkybox(json scene)
{
    Skybox* skybox = new Skybox();
    std::vector<std::string> faces = scene["skybox"]["faces"];
    skybox->LoadCubeMap(faces);
    RenderManager::GetInstance().SetSkybox(skybox);
}

void SceneManager::LoadModels(json scene)
{
    SceneNode* root = LoadChild(scene["models"]);
    CoreManager::GetInstance().SetRootNode(root);
}

SceneNode* LoadChild(json root) {
    std::string name = root["name"];
    SceneNode* node = new SceneNode(name);
    if (root["transform"] != nullptr) {
        json t = root["transform"];
        if (t["position"] != nullptr) {
            std::vector<float> p = t["position"];
            node->GetTransform().MoveTo(glm::vec3(p[0], p[1], p[2]));
        }
        if (t["rotation"] != nullptr) {
            std::vector<float> r = t["rotation"];
            node->GetTransform().SetRotation(r[0], r[1], r[2]);
        }
        if (t["scale"] != nullptr) {
            std::vector<float> s = t["scale"];
            node->GetTransform().SetScale(glm::vec3(s[0], s[1], s[2]));
        }
    }
    if (root["model"] != nullptr) {
        std::string modelPath = root["model"];
        Model* model = Model::CachedModels[modelPath];
        if (model == nullptr) {
            model = new Model(modelPath);
            Model::CachedModels[modelPath] = model;
        }
        else {
            model->IncrementInstances();
        }
        node->AddComponent(model);
    }
    if (root["components"] != nullptr) {
        std::vector<json> components = root["components"];
        for (int i = 0; i < components.size(); i++) {
            json c = components[i];
            if (c["name"] == "DirectionalLight") {
                DirectionalLight* light = new DirectionalLight();
                json d = c["data"];
                if (d["direction"] != nullptr) {
                    std::vector<float> dir = d["direction"];
                    light->SetDirection(glm::vec3(dir[0], dir[1], dir[2]));
                }
                if (d["color"] != nullptr) {
                    std::vector<float> col = d["color"];
                    light->SetColor(glm::vec3(col[0], col[1], col[2]));
                }
                if (d["intensity"] != nullptr) {
                    float intensity = d["intensity"];
                    light->SetIntensity(intensity);
                }
                node->AddComponent(light);
            }
            else if (c["name"] == "PointLight") {
                PointLight* light = new PointLight(&node->GetTransform());
                json d = c["data"];
                if (d["range"] != nullptr) {
                    float range = d["range"];
                    light->SetRange(range);
                }
                if (d["color"] != nullptr) {
                    std::vector<float> col = d["color"];
                    light->SetColor(glm::vec3(col[0], col[1], col[2]));
                }
                if (d["intensity"] != nullptr) {
                    float intensity = d["intensity"];
                    light->SetIntensity(intensity);
                }
                node->AddComponent(light);
            }
        }
    }
    if (root["children"] != nullptr) {
        std::vector<json> children = root["children"];
        for (int i = 0; i < children.size(); i++) {
            node->AppendChild(LoadChild(children[i]));
        }
    }
    return node;
}
