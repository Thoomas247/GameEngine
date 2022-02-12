#pragma once

#include "../ECS/Scene.h"

class SceneManager
{
private:
	static Scene* s_CurrentScene;

public:
	static uint64_t AddEntity(const std::string& name) { return s_CurrentScene->AddEntity(name); }
	static void RemoveEntity(const uint64_t& entityID) { s_CurrentScene->RemoveEntity(entityID); }
	static Entity* GetEntity(const uint64_t& entityID) { return s_CurrentScene->GetEntity(entityID); }
};

