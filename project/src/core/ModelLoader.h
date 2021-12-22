#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <memory>
#include <string>
#include <vector>

#include "json/json.hpp"
using json = nlohmann::json;

#include "../core/GameObject.h"
#include "../renderer/Skeleton.h"
#include "../renderer/MeshData.h"
#include "../structs/Vertex.h"
#include "../structs/RenderData.h"

namespace ModelLoader
{
	std::shared_ptr<GameObject> LoadModel(const std::string& name);

	int loadTexture(const std::string& name);
	std::shared_ptr<Skeleton> createSkeleton(json& j);
	RenderData createRenderData(json& jmesh);
	std::shared_ptr<MeshData> createMeshData(json& jmesh);
}

#endif // !ASSET_MANAGER