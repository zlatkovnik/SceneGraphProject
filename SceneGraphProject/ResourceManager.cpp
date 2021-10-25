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
    //if (assets["shaders"].is_array()) {
    //    LoadShaders(assets["shaders"]);
    //}
}

void ResourceManager::LoadShaders(json shaders)
{
    for (std::string shader : shaders) {
        Shader s((shader + ".vert").c_str(), (shader + ".frag").c_str());
        ShaderLookup[shader] = s;
    }
}

void ResourceManager::LoadTextures(json textures)
{
    for (std::string texture : textures) {
        unsigned int texId;
        glGenTextures(1, &texId);
        stbi_set_flip_vertically_on_load(true);
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
