#include "SceneManager.h"

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
