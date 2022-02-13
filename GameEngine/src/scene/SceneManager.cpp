#include "SceneManager.h"

#include <fstream>

#include "json/json.hpp"
using json = nlohmann::json;
#include "glm/gtx/matrix_decompose.hpp"

#include "../core/MatrixUtil.h"

Scene* SceneManager::s_CurrentScene = nullptr;
std::unordered_map<uint64_t, Scene> SceneManager::s_Scenes;

/* -- PUBLIC -- */

void SceneManager::Update(const float& deltaTime)
{
	s_CurrentScene->Update(deltaTime);
}

void SceneManager::CreateScene(const std::string name)
{
	Scene scene = Scene(name);
	s_Scenes[scene.GetUUID()] = scene;
	s_CurrentScene = &s_Scenes[scene.GetUUID()];
}

void SceneManager::LoadScene(const std::string& absolutePath)
{
	size_t dotPos = absolutePath.find_last_of(".");
	size_t slashPos = absolutePath.find_last_of("/");
	if (dotPos == absolutePath.npos || slashPos == absolutePath.npos || slashPos > dotPos)
	{
		LOG_ERROR("SCENE_MANAGER::Invalid scene path!");
	}
	slashPos++;

	std::string sceneName = absolutePath.substr(slashPos, dotPos - slashPos);

	// TODO: Add automatic scene saving on new one load
	//		 Add m_Path member to Scene
	CreateScene(sceneName);

	// load from file
	std::streamsize size = std::filesystem::file_size(absolutePath);
	std::vector<char> buffer;
	buffer.resize(size);

	std::ifstream fileIn = std::ifstream(absolutePath, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	json j = json::parse(buffer);

	// entities
	for (auto& [key, jEntity] : j["entities"].items())
	{
		auto entity = AddEntity(jEntity["name"]);

		// transform component
		if (!jEntity["transformComponent"].is_null())
		{
			std::string transformID = jEntity["transformComponent"];
			std::vector<double> localTransformVec = j["transformComponents"][transformID]["localTransform"];

			glm::mat4 localTransform = MatrixUtil::ToMat4(localTransformVec);
			glm::vec3 scale = glm::vec3();
			glm::quat rotation = glm::quat();
			glm::vec3 translation = glm::vec3();
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(localTransform, scale, rotation, translation, skew, perspective);

			entity->AddTransformComponent(translation, rotation, scale);
		}

		// mesh component
		if (!jEntity["meshComponent"].is_null())
		{
			std::string meshID = jEntity["meshComponent"];

			// vertex array asset
			std::string vaoID = j["meshComponents"][meshID]["vertexArrayAsset"];

			VertexArrayAsset vao;
			if (!j["vertexArrayAssets"][vaoID].is_null())
			{
				// vertices
				std::vector<Vertex> vertices;
				std::vector<float> vertFloats = j["vertexArrayAssets"][vaoID]["vertices"];
				int arraySize = (int)vertFloats.size() / 20; // 20 is num of floats in a vertex // TODO: Store this value in vertex struct itself
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

				// indices
				std::vector<unsigned int> indices = j["vertexArrayAssets"][vaoID]["indices"];

				vao = GraphicsAssetManager::LoadVertexArray(vertices, indices);
			}
			else
			{
				LOG_WARN("SCENE_MANAGER::Vertex array asset was empty. Creating default square.");
				vao = GraphicsAssetManager::LoadVertexArray();
			}

			// material
			json jMat = j["meshComponents"][meshID]["material"];

			glm::vec4 baseF = glm::vec4(jMat["baseColorFactor"][0], jMat["baseColorFactor"][1], jMat["baseColorFactor"][2], jMat["baseColorFactor"][3]);
			glm::vec3 esmissiveF = glm::vec3(jMat["emissiveFactor"][0], jMat["emissiveFactor"][1], jMat["emissiveFactor"][2]);
			float metallicF = jMat["metallicFactor"];
			float roughnessF = jMat["roughnessFactor"];
			TextureAsset baseT = GraphicsAssetManager::LoadTexture(jMat["baseColorTexture"]);
			TextureAsset emissiveT = GraphicsAssetManager::LoadTexture(jMat["emissiveTexture"]);
			TextureAsset metallicRoughnessT = GraphicsAssetManager::LoadTexture(jMat["metallicRoughnessTexture"]);
			TextureAsset normalT = GraphicsAssetManager::LoadTexture(jMat["normalTexture"]);
			TextureAsset occlusionT = GraphicsAssetManager::LoadTexture(jMat["occlusionTexture"]);

			Material mat = Material(baseF, esmissiveF, metallicF, roughnessF, baseT, emissiveT, metallicRoughnessT, normalT, occlusionT);

			entity->AddMeshComponent(vao, GraphicsAssetManager::LoadShader(), mat);
		}
	}
}

void SceneManager::LoadSubScene(const std::string& absolutePath, const uint64_t& parentID)
{
	size_t dotPos = absolutePath.find_last_of(".");
	size_t slashPos = absolutePath.find_last_of("/");
	if (dotPos == absolutePath.npos || slashPos == absolutePath.npos || slashPos > dotPos)
	{
		LOG_ERROR("SCENE_MANAGER::Invalid scene path!");
	}
	slashPos++;

	std::string sceneName = absolutePath.substr(slashPos, dotPos - slashPos);

	// this entity is the root of the scene that is being loaded into the current scene
	auto entity = AddEntity(sceneName, parentID);
	uint64_t sceneRootID = entity->GetUUID();

	// load from file
	std::streamsize size = std::filesystem::file_size(absolutePath);
	std::vector<char> buffer;
	buffer.resize(size);

	std::ifstream fileIn = std::ifstream(absolutePath, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	json j = json::parse(buffer);

	// entities
	for (auto& [key, jEntity] : j["entities"].items())
	{
		// add this new entity to the root of the new scene, not of the current scene
		auto entity = AddEntity(jEntity["name"], sceneRootID);

		// transform component
		if (!jEntity["transformComponent"].is_null())
		{
			std::string transformID = jEntity["transformComponent"];
			std::vector<double> localTransformVec = j["transformComponents"][transformID]["localTransform"];

			glm::mat4 localTransform = MatrixUtil::ToMat4(localTransformVec);
			glm::vec3 scale = glm::vec3();
			glm::quat rotation = glm::quat();
			glm::vec3 translation = glm::vec3();
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(localTransform, scale, rotation, translation, skew, perspective);

			entity->AddTransformComponent(translation, rotation, scale);
		}

		// mesh component
		if (!jEntity["meshComponent"].is_null())
		{
			std::string meshID = jEntity["meshComponent"];

			// vertex array asset
			std::string vaoID = j["meshComponents"][meshID]["vertexArrayAsset"];

			VertexArrayAsset vao;
			if (!j["vertexArrayAssets"][vaoID].is_null())
			{
				// vertices
				std::vector<Vertex> vertices;
				std::vector<float> vertFloats = j["vertexArrayAssets"][vaoID]["vertices"];
				int arraySize = (int)vertFloats.size() / 20; // 20 is num of floats in a vertex // TODO: Store this value in vertex struct itself
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

				// indices
				std::vector<unsigned int> indices = j["vertexArrayAssets"][vaoID]["indices"];

				vao = GraphicsAssetManager::LoadVertexArray(vertices, indices);
			}
			else
			{
				LOG_WARN("SCENE_MANAGER::Vertex array asset was empty. Creating default square.");
				vao = GraphicsAssetManager::LoadVertexArray();
			}

			// material
			json jMat = j["meshComponents"][meshID]["material"];

			glm::vec4 baseF = glm::vec4(jMat["baseColorFactor"][0], jMat["baseColorFactor"][1], jMat["baseColorFactor"][2], jMat["baseColorFactor"][3]);
			glm::vec3 esmissiveF = glm::vec3(jMat["emissiveFactor"][0], jMat["emissiveFactor"][1], jMat["emissiveFactor"][2]);
			float metallicF = jMat["metallicFactor"];
			float roughnessF = jMat["roughnessFactor"];
			TextureAsset baseT = GraphicsAssetManager::LoadTexture(jMat["baseColorTexture"]);
			TextureAsset emissiveT = GraphicsAssetManager::LoadTexture(jMat["emissiveTexture"]);
			TextureAsset metallicRoughnessT = GraphicsAssetManager::LoadTexture(jMat["metallicRoughnessTexture"]);
			TextureAsset normalT = GraphicsAssetManager::LoadTexture(jMat["normalTexture"]);
			TextureAsset occlusionT = GraphicsAssetManager::LoadTexture(jMat["occlusionTexture"]);

			Material mat = Material(baseF, esmissiveF, metallicF, roughnessF, baseT, emissiveT, metallicRoughnessT, normalT, occlusionT);

			entity->AddMeshComponent(vao, GraphicsAssetManager::LoadShader(), mat);
		}
	}
}