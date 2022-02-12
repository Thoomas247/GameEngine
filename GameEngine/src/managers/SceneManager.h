#pragma once

#include <unordered_map>

#include "../ECS/Scene.h"

class SceneManager
{
private:
	static Scene* s_CurrentScene;
	static std::unordered_map<uint64_t, Scene> s_Scenes;

public:
	static void Update(const float& deltaTime);
	static void CreateScene(const std::string name = "New Scene");

	static Entity* AddEntity(const std::string& name) { return s_CurrentScene->AddEntity(name); }
	static void RemoveEntity(const uint64_t& entityID) { s_CurrentScene->RemoveEntity(entityID); }
	static Entity* GetEntity(const uint64_t& entityID) { return s_CurrentScene->GetEntity(entityID); }
};

