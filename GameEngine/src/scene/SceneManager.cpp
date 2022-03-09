#include "precompiled.h"
#include "SceneManager.h"

#include "../ECS/components/TransformComponent.h"
#include "../ECS/components/MeshComponent.h"


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
	json j;
	std::string sceneName;
	size_t dotPos;
	size_t slashPos;
	getSceneFileData(absolutePath, j, sceneName, dotPos, slashPos);

	// TODO: Add automatic scene saving on new one loaded
	//		 Add m_Path member to Scene
	CreateScene(sceneName);

	// entities
	for (auto& [key, jEntity] : j["entities"].items())
	{
		auto entity = CreateEntityAtRoot(jEntity["name"]);

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

			entity->AddComponent<TransformComponent>(translation, rotation, scale);
		}

		// mesh component
		if (!jEntity["meshComponent"].is_null())
		{
			std::string meshID = jEntity["meshComponent"];
			entity->AddComponent<MeshComponent>(j["meshComponents"][meshID]);
		}
	}
}

void SceneManager::LoadSubScene(const std::string& absolutePath, const std::shared_ptr<Entity>& parent)
{
	json j;
	std::string sceneName;
	size_t dotPos;
	size_t slashPos;
	getSceneFileData(absolutePath, j, sceneName, dotPos, slashPos);

	// this entity is the root of the scene that is being loaded into the current scene
	auto rootEntity = parent->CreateChild(sceneName);

	// entities
	for (auto& [key, jEntity] : j["entities"].items())
	{
		// add this new entity to the root of the new scene, not of the current scene
		auto entity = rootEntity->CreateChild(jEntity["name"]);

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

			entity->AddComponent<TransformComponent>(translation, rotation, scale);
		}

		// mesh component
		if (!jEntity["meshComponent"].is_null())
		{
			std::string meshID = jEntity["meshComponent"];
			entity->AddComponent<MeshComponent>(j["meshComponents"][meshID]);
		}
	}
}


/* -- PRIVATE -- */

void SceneManager::getSceneFileData(const std::string& absolutePath, json& j, std::string& sceneName, size_t& dotPos, size_t& slashPos)
{
	dotPos = absolutePath.find_last_of(".");
	slashPos = absolutePath.find_last_of("/");
	if (dotPos == absolutePath.npos || slashPos == absolutePath.npos || slashPos > dotPos)
	{
		LOG_ERROR("SCENE_MANAGER::Invalid scene path!");
	}
	slashPos++;

	sceneName = absolutePath.substr(slashPos, dotPos - slashPos);

	// load from file
	std::streamsize size = std::filesystem::file_size(absolutePath);
	std::vector<char> buffer;
	buffer.resize(size);

	std::ifstream fileIn = std::ifstream(absolutePath, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	j = json::parse(buffer);
}
