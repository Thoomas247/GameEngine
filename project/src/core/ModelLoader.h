#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <memory>
#include <string>
#include <unordered_map>

#include "json/json.hpp"
using json = nlohmann::json;

#include "../core/GameObject.h"
#include "../renderer/Skeleton.h"
#include "../renderer/MeshData.h"
#include "../structs/Vertex.h"
#include "../structs/RenderData.h"

struct CacheData
{
	std::string MeshName;
	RenderData RenderDataCache;
	std::shared_ptr<MeshData> MeshDataCache;
	std::shared_ptr<Skeleton> SkeletonCache;
	glm::mat4 Transform;

	CacheData(const std::string& meshName, RenderData renderData /*copied*/, const std::shared_ptr<MeshData>& meshData, const std::shared_ptr<Skeleton>& skeleton, const glm::mat4& transform)
	{
		MeshName = meshName;
		RenderDataCache = renderData;
		MeshDataCache = meshData;
		SkeletonCache = skeleton;
		Transform = transform;
	}
};

namespace ModelLoader
{
	extern std::unordered_map<std::string, int> g_TextureCache;
	extern std::unordered_map<std::string, std::vector<std::unique_ptr<CacheData>>> g_ModelCache;

	std::shared_ptr<GameObject> LoadModel(const std::string& name);

	int loadTexture(const std::string& name);
	std::shared_ptr<Skeleton> createSkeleton(json& j);
	RenderData createRenderData(json& jmesh);
	std::shared_ptr<MeshData> createMeshData(json& jmesh);
}

#endif // !ASSET_MANAGER