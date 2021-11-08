#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <string>
#include "Shader.h"
#include "Model.h"
#include "DataStructures.h"

using namespace nlohmann;
class ResourceManager
{
public:
	std::unordered_map<std::string, Shader> ShaderLookup;
	std::unordered_map<std::string, unsigned int> TextureLookup;
	std::unordered_map<std::string, Mesh> MeshLookup;
	std::unordered_map<std::string, Material> MaterialLookup;

	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	static ResourceManager& GetInstance();

	void LoadAssets(json assets);
private:
	ResourceManager() {}
	void LoadShaders(json shaders);
	void LoadTextures(json textures);
	void LoadMaterials(json materials);
};

