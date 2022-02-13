#pragma once

#include <unordered_map>
#include <memory>

#include "../ECS/Scene.h"

class SceneManager
{
private:
	static Scene* s_CurrentScene;
	static std::unordered_map<uint64_t, Scene> s_Scenes;

public:
	static void Update(const float& deltaTime);
	static void CreateScene(const std::string name = "New Scene");
	static void LoadScene(const std::string& absolutePath);
	static void LoadSubScene(const std::string& absolutePath);

	static std::shared_ptr<Entity> AddEntity(const std::string& name, const uint64_t& parentID = 0) { return s_CurrentScene->AddEntity(name, parentID); }
	static void RemoveEntity(const uint64_t& entityID) { s_CurrentScene->RemoveEntity(entityID); }
	static std::shared_ptr<Entity> GetEntity(const uint64_t& entityID) { return s_CurrentScene->GetEntity(entityID); }
};

