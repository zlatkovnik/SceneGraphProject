#pragma once
#include <string>
#include <vector>
#include "Shader.h"
class Skybox
{
private:
    static float SKYBOX_VERTICES[];
    unsigned int m_textureId;
    unsigned int m_vao;
    unsigned int m_vbo;
public:
    Skybox(std::vector<std::string> faces);
    void Draw();
private:
    void LoadCubeMap(std::vector<std::string> faces);
};

