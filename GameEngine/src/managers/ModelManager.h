#pragma once

#include <memory>
#include <string>
#include <map>

#include "json/json.hpp"
using json = nlohmann::json;
#include "glm/glm.hpp"

#include "../structs/Vertex.h"
#include "../structs/Model.h"

class ModelManager
{
private:
	static std::map<std::string, Model> s_ModelCache;

public:
	static std::shared_ptr<GameObject> LoadModel(const std::string& modelPath);

private:
	static Skeleton createSkeleton(json& j);
	static Mesh createMesh(json& jmesh);
};
