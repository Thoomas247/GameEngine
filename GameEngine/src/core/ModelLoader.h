#pragma once

#include <memory>
#include <string>
#include <map>

#include "json/json.hpp"
using json = nlohmann::json;
#include "glm/glm.hpp"

#include "../renderer/Mesh.h"
#include "../structs/Vertex.h"

// TODO: make folder for loader and organize into files

struct Model
{
	std::map<std::string, Mesh> m_Meshes;
	Skeleton m_Skeleton;

	Model()
	{
	}

	Model(const std::map<std::string, Mesh>& meshes, const Skeleton& skeleton)
	{
		m_Meshes = meshes;
		m_Skeleton = skeleton;
	}
};

namespace ModelLoader
{
	extern std::map<std::string, int> TextureCache;	// TODO: Move to TextureLibrary
	extern std::map<std::string, Model> ModelCache;

	std::shared_ptr<GameObject> LoadModel(const std::string& modelPath);
}