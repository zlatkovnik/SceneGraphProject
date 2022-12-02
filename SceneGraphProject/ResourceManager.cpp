#include "ResourceManager.h"
#include <stb/stbi_image.h>

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadAssets(json assets)
{
    if (assets["shaders"].is_array()) {
        LoadShaders(assets["shaders"]);
    }
    if (assets["textures"].is_array()) {
        LoadTextures(assets["textures"]);
    }
    if (assets["materials"].is_array()) {
        LoadMaterials(assets["materials"]);
    }
}

void ResourceManager::LoadShaders(json shaders)
{
    for (auto shader : shaders) {
        std::string vertex = shader["vertex"];
        std::string fragment = shader["fragment"];
        Shader s(vertex.c_str(), fragment.c_str());
        s.SetShading(shader["shadingEnabled"].is_null() ? true : (bool)shader["shadingEnabled"]);
        s.SetTimeInput(shader["timeInput"].is_null() ? false : (bool)shader["timeInput"]);
        std::string name = shader["name"];
        bool customData = shader["customData"].is_null() ? false : (bool)shader["customData"];
        if (customData) {
            s.SetCustomData(customData);
            json data = shader["data"];
            for (auto it : data.items()) {
                std::string type = it.value()["type"];
                if (type == "vec3") {
                    std::vector<float> vec = it.value()["value"];
                    glm::vec3* value = new glm::vec3();
                    value->x = vec[0];
                    value->y = vec[1];
                    value->z = vec[2];
                    UniformData ud(it.key(), type, value);
                    s.AddCustomData(ud.m_name, ud);
                }
                else {
                    std::cout << "[ResourceManager] Type '"  << type << "' does not exist" << std::endl;
                }
            }
        }
        ShaderLookup[name] = s;
    }
}

void ResourceManager::LoadTextures(json textures)
{
    stbi_set_flip_vertically_on_load(true);
    for (std::string texture : textures) {
        unsigned int texId;
        glGenTextures(1, &texId);
        int width, height, nrComponents;
        unsigned char* data = stbi_load(texture.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texId);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            TextureLookup[texture] = texId;

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << texture << std::endl;
            stbi_image_free(data);
        }
    }
}

void ResourceManager::LoadMaterials(json materials)
{
    for (auto material : materials) {
        Shader* shader = &ShaderLookup[material["shader"]];
        Material m(
            material["name"],
            glm::vec3(material["ambient"][0], material["ambient"][1], material["ambient"][2]),
            glm::vec3(material["diffuse"][0], material["diffuse"][1], material["diffuse"][2]),
            glm::vec3(material["specular"][0], material["specular"][1], material["specular"][2]),
            material["shininess"], shader);
        MaterialLookup[material["name"]] = m;
    }
}
