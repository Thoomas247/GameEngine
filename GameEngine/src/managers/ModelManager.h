#pragma once

#include <memory>
#include <string>
#include <map>

#include "json/json.hpp"
using json = nlohmann::json;
#include "glm/glm.hpp"

#include "../renderer/Mesh.h"
#include "../structs/Vertex.h"

// TODO: make folder for loader and organize classes into files

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

namespace ModelManager
{
	extern std::map<std::string, Model> ModelCache;

	std::shared_ptr<GameObject> LoadModel(const std::string& modelPath);
}

Skeleton createSkeleton(json& j);
Mesh createMesh(json& jmesh);