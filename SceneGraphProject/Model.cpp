#include "Model.h"
#include <stb/stbi_image.h>
#include "ResourceManager.h"

Model::Model(std::string const& path, Material* material)
    :m_material(material)
{
    LoadModel(path);
}

Model::Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Material* material, bool normalsIncluded)
    :m_material(material)
{
    LoadVertices(vertices, indices, normalsIncluded);
}

void Model::Render()
{
    m_material->SetMaterial();
    for (unsigned int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].Draw(*m_material->GetShader(), m_instances, m_wireframe);
    }
}

void Model::IncrementInstances()
{
    m_instances++;
}

void Model::SetMaterial(Material* material)
{
    m_material = material;
}

Material* Model::GetMaterial()
{
    return m_material;
}

void Model::SetWireframe(bool wireframe)
{
    m_wireframe = wireframe;
}

bool Model::GetWireframe() const
{
    return m_wireframe;
}

int Model::GetNumberOfInstances() const
{
    return m_instances;
}

std::unordered_map<std::string, Model*> Model::CachedModels;

void Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    bool hasNormals = true;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.m_position = vector;

        if (mesh->mNormals != nullptr) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.m_normal = vector;
        }
        else {
            hasNormals = false;
            vertex.m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.m_texCoords = vec;
        } else
            vertex.m_texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        auto faceIndices = std::vector<unsigned int>();
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
            faceIndices.push_back(face.mIndices[j]);
        }

        if (!hasNormals) {
            auto a = vertices[faceIndices[0]].m_position;
            auto b = vertices[faceIndices[1]].m_position;
            auto c = vertices[faceIndices[2]].m_position;
            auto dir = glm::cross(b - a, c - a);
            auto normal = glm::normalize(dir);
            for (unsigned int j = 0; j < faceIndices.size(); j++) {
                vertices[faceIndices[j]].m_normal = normal;
            }
        }
    }
    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        unsigned int texId = ResourceManager::GetInstance().TextureLookup[m_directory + "/" + str.C_Str()];
        Texture texture;
        texture.m_id = texId;
        texture.m_path = m_directory + str.C_Str();
        texture.m_type = typeName;
        textures.push_back(texture);
    }
    return textures;
}

void Model::LoadVertices(const std::vector<float>& inputVertices, const std::vector<unsigned int>& inputIndices, bool normalsIncluded)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int i = 0;
    while (i < inputVertices.size()) {
        Vertex vertex;
        vertex.m_position.x = inputVertices[i++];
        vertex.m_position.y = inputVertices[i++];
        vertex.m_position.z = inputVertices[i++];

        if (normalsIncluded) {
            vertex.m_normal.x = inputVertices[i++];
            vertex.m_normal.y = inputVertices[i++];
            vertex.m_normal.z = inputVertices[i++];
        }
        else {
            i += 3;
        }

        vertex.m_texCoords.x = 0.0f;
        vertex.m_texCoords.y = 0.0f;
        i += 2;

        vertices.push_back(vertex);
    }
    indices = inputIndices;
    for (unsigned int i = 0; i < indices.size(); i+=3) {
        if (!normalsIncluded) {
            auto a = vertices[indices[i]].m_position;
            auto b = vertices[indices[i + 1]].m_position;
            auto c = vertices[indices[i + 2]].m_position;
            auto dir = glm::cross(b - a, c - a);
            auto normal = glm::normalize(dir);

            vertices[indices[i]].m_normal = normal;
            vertices[indices[i + 1]].m_normal = normal;
            vertices[indices[i + 2]].m_normal = normal;
        }
    }
    std::vector<Texture> dummy;
    Mesh mesh(vertices, indices, dummy);
    m_meshes.push_back(mesh);
}
