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
#include "ResourceManager.h"
#include "Collider.h"

SceneNode* LoadChild(json root);

SceneManager& SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::LoadScene(const char* path, const char* title, unsigned int screenWidth, unsigned int screenHeight, bool fullScreen)
{
    CoreManager::GetInstance().Init(screenWidth, screenHeight, title, fullScreen);

    std::ifstream stream(path);
    json scene;
    stream >> scene;

    ResourceManager::GetInstance().LoadAssets(scene["assets"]);
    //auto manager = &ResourceManager::GetInstance();
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
        json model = root["model"];
        Model* m = Model::CachedModels[model["path"]];
        if (m == nullptr) {
            Material* material;
            if (model["material"].is_null()) {
                material = &ResourceManager::GetInstance().MaterialLookup["standard"];
            }
            else {
                material = &ResourceManager::GetInstance().MaterialLookup[model["material"]];
            }
            m = new Model(model["path"], material);
            Model::CachedModels[model["path"]] = m;
        }
        else {
            m->IncrementInstances();
        }
        node->AddComponent(m);
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
            else if (c["name"] == "Collider") {
                json d = c["data"];
                std::vector<float> max = d["max"];
                glm::vec3 maxV = glm::vec3(max[0], max[1], max[2]);
                std::vector<float> min = d["min"];
                glm::vec3 minV = glm::vec3(min[0], min[1], min[2]);
                Collider* collider = new Collider(maxV, minV, node);
                node->AddComponent(collider);
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
