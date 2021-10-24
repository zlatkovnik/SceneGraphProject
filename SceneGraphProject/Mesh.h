#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Material.h"

struct Vertex {
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_texCoords;
};

struct Texture {
    unsigned int m_id;
    std::string m_type;
    std::string m_path;
};

class Mesh
{
private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    Material* m_material;
public:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture>      m_textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader, int instances = 1);
    void SetMaterial(std::string name);
    void SetMaterial(Material* material);
private:
    void SetupMesh();
};

