#pragma once
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "Component.h"
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model: public Component
{
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_texturesLoaded;
public:
    Model(std::string const& path, bool gamma = false);
    void Render(Shader shader) override;

    static std::unordered_map<std::string, Model*> CachedModels;
private:

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

