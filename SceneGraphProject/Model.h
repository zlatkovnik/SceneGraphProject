#pragma once
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "Component.h"
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model: public Component
{
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    Material* m_material;
    int m_instances = 1;
    bool m_wireframe = false;
public:
    Model(std::string const& path, Material* material);
    Model(const std::vector<float> &vertices, const std::vector<unsigned int>& indices, Material* material, bool normalsIncluded = false);
    void Render();

    static std::unordered_map<std::string, Model*> CachedModels;
    void IncrementInstances();

    void SetMaterial(Material *material);
    Material* GetMaterial();

    void SetWireframe(bool wireframe);
    bool GetWireframe() const;

    int GetNumberOfInstances() const;

private:

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    void LoadVertices(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, bool normalsIncluded);
};

