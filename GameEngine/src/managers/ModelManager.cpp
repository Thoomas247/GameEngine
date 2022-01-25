#include "ModelManager.h"

#include <fstream>

#include "glad/gl.h"
#include "stb/stb_image.h"

#include "ProjectManager.h"
#include "TextureManager.h"
#include "../core/Log.h"
#include "../renderer/Mesh.h"

std::map<std::string, Model> ModelManager::ModelCache;

std::shared_ptr<GameObject> ModelManager::LoadModel(const std::string& modelPath)
{
	//std::string modelName = modelPath.substr(modelPath.find_last_of("/") + 1);
	//modelName.resize(modelName.find_last_of("."));

	// check if model is in cache
	auto it = ModelCache.find(modelPath);
	if (it != ModelCache.end())
	{
		Model& model = it->second;
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

		auto skeleton = std::make_shared<Skeleton>(model.m_Skeleton);
		gameObject->AddChild("Skeleton", skeleton);

		for (const auto& [name, savedMesh] : model.m_Meshes)
		{
			auto mesh = std::make_shared<Mesh>(savedMesh);
			mesh->m_Skeleton = skeleton;
			gameObject->AddChild(name, mesh);
		}

		return gameObject;
	}

	// load from file
	std::streamsize size = std::filesystem::file_size(modelPath);
	std::vector<char> buffer;
	buffer.resize(size);

	std::ifstream fileIn = std::ifstream(modelPath, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	json j = json::from_bson(buffer);

	std::map<std::string, Mesh> meshes;
	for (auto& [name, jmesh] : j["meshes"].items())
	{
		meshes[name] = createMesh(jmesh);
	}

	Model model = Model(meshes, createSkeleton(j));

	ModelCache[modelPath] = model;
	return LoadModel(modelPath);
}

Skeleton createSkeleton(json& j)
{
	std::vector<Joint> skeletonJoints;
	std::map<std::string, Animation> animations;
	if (!j["joints"].is_null())
	{
		// skeleton
		glm::mat4 matrix;
		for (unsigned int i = 0; i < j["joints"].size(); i++)
		{
			std::vector<float> vec = j["joints"][i]["inverseBindMatrix"];
			matrix = glm::mat4(vec[0], vec[1], vec[2], vec[3],
				vec[4], vec[5], vec[6], vec[7],
				vec[8], vec[9], vec[10], vec[11],
				vec[12], vec[13], vec[14], vec[15]);

			skeletonJoints.push_back(Joint(j["joints"][i]["parentID"], matrix));
		}

		// animations
		for (auto& [name, anim] : j["animations"].items())
		{
			std::map<float, std::map<int, glm::mat4>> keyframes;	// timestamp, joint id, matrix transform
			glm::mat4 transform;
			for (auto& [time, targets] : anim.items())
			{
				for (int i = 0; i < targets.size(); i++)
				{
					json& target = targets[i];

					if (target.is_null())
					{
						transform = glm::mat4(1.0f);
					}
					else
					{
						glm::vec3 translation = glm::vec3(target["translation"][0], target["translation"][1], target["translation"][2]);
						glm::quat rotation = glm::quat(target["rotation"][0], target["rotation"][1], target["rotation"][2], target["rotation"][3]);
						glm::vec3 scale = glm::vec3(target["scale"][0], target["scale"][1], target["scale"][2]);

						transform = glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);

						keyframes[std::stof(time)][i] = transform;
					}
				}
			}
			animations[name] = Animation(keyframes);
		}
	}
	else
	{
		LOG_WARN("MODEL_LOADER::Model has no skeleton")
	}

	return Skeleton(skeletonJoints, animations);
}

Mesh createMesh(json& jmesh)
{
	// buffer data
	std::vector<float> vertFloats = jmesh["vertices"];

	std::vector<Vertex> vertices;
	int arraySize = (int)vertFloats.size() / 20; // 20 is num of floats in a vertex
	vertices.reserve(arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		vertices.push_back(Vertex(glm::vec3(vertFloats[i * 20 + 0], vertFloats[i * 20 + 1], vertFloats[i * 20 + 2]),
			glm::vec3(vertFloats[i * 20 + 3], vertFloats[i * 20 + 4], vertFloats[i * 20 + 5]),
			glm::vec2(vertFloats[i * 20 + 6], vertFloats[i * 20 + 7]),
			glm::vec4(vertFloats[i * 20 + 8], vertFloats[i * 20 + 9], vertFloats[i * 20 + 10], vertFloats[i * 20 + 11]),
			glm::ivec4(vertFloats[i * 20 + 12], vertFloats[i * 20 + 13], vertFloats[i * 20 + 14], vertFloats[i * 20 + 15]),
			glm::vec4(vertFloats[i * 20 + 16], vertFloats[i * 20 + 17], vertFloats[i * 20 + 18], vertFloats[i * 20 + 19])));
	}

	std::vector<unsigned int> indices = jmesh["indices"];

	auto vertexArray = std::make_shared<VertexArray>(vertices, indices);


	// material
	std::vector<float> baseColor = jmesh["baseColorFactor"];
	std::vector<float> emissiveColor = jmesh["emissiveFactor"];

	std::string basePath = ProjectManager::GetTexturesPath() + std::string(jmesh["baseColorTexture"]);
	std::string emissivePath = ProjectManager::GetTexturesPath() + std::string(jmesh["emissiveTexture"]);
	std::string metallicRoughnessPath = ProjectManager::GetTexturesPath() + std::string(jmesh["metallicRoughnessTexture"]);
	std::string normalPath = ProjectManager::GetTexturesPath() + std::string(jmesh["normalTexture"]);
	std::string occlusionPath = ProjectManager::GetTexturesPath() + std::string(jmesh["occlusionTexture"]);

	std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]), glm::vec3(emissiveColor[0], emissiveColor[1], emissiveColor[2]),
		jmesh["metallicFactor"], jmesh["roughnessFactor"], TextureManager::LoadTexture(basePath), TextureManager::LoadTexture(emissivePath), TextureManager::LoadTexture(metallicRoughnessPath), 
		TextureManager::LoadTexture(normalPath), TextureManager::LoadTexture(occlusionPath));

	// transform
	std::vector<float> transformVec = jmesh["transform"];
	glm::mat4 transform = { transformVec[0], transformVec[4], transformVec[8], transformVec[12],
							transformVec[1], transformVec[5], transformVec[9], transformVec[13],
							transformVec[2], transformVec[6], transformVec[10], transformVec[14],
							transformVec[3], transformVec[7], transformVec[11], transformVec[15] };


	return Mesh(vertexArray, nullptr, material, std::make_shared<Shader>("assets/shaders/Base.vert", "assets/shaders/Base.frag"), transform);
}
