#pragma once

#include <memory>
#include <string>
#include <map>

#include "json/json.hpp"
using json = nlohmann::json;
#include "glm/glm.hpp"

#include "../core/GameObject.h"
#include "../renderer/Skeleton.h"
#include "../renderer/MeshData.h"
#include "../renderer/RenderData.h"
#include "../structs/Vertex.h"

// TODO: make folder for loader and organize into files

struct Model
{
	struct MeshCache
	{
		std::string m_Name;
		RenderData m_RenderData;
		std::shared_ptr<MeshData> m_MeshData;	// can be shared accross many meshes so is shared_ptr
		glm::mat4 m_Transform = glm::mat4(1.0f);

		MeshCache()
		{
		}

		MeshCache(const std::string& name, const RenderData& renderData, const std::shared_ptr<MeshData>& meshData, const glm::mat4& transform)
		{
			m_Name = name;
			m_RenderData = renderData;
			m_MeshData = meshData;
			m_Transform = transform;
		}
	};

	struct SkeletonCache
	{
		std::vector<Joint> m_Joints;
		std::map<std::string, Animation> m_Animations;

		SkeletonCache()
		{
		}

		SkeletonCache(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations)
		{
			m_Joints = joints;
			m_Animations = animations;
		}
	};

	std::vector<MeshCache> m_Meshes;
	SkeletonCache m_Skeleton;

	Model()
	{
	}
};

namespace ModelLoader
{
	extern std::map<std::string, int> g_TextureCache;	// TODO: Move to TextureLibrary
	extern std::map<std::string, Model> g_ModelCache;

	std::shared_ptr<GameObject> LoadModel(const std::string& modelPath);

	int loadTexture(const std::string& name);	// TODO: change to taking in path
	RenderData createRenderData(json& jmesh);
	std::shared_ptr<MeshData> createMeshData(json& jmesh);
	glm::mat4 createTransform(json& jmesh);
	Model::SkeletonCache createSkeleton(json& j);
}