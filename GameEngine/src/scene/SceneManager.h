#pragma once

#include <unordered_map>
#include <memory>

#include "../scene/Scene.h"

class SceneManager
{
private:
	static Scene* s_CurrentScene;
	static std::unordered_map<uint64_t, Scene> s_Scenes;

public:
	static void Update(const float& deltaTime);
	static void CreateScene(const std::string name = "New Scene");
	/// <summary>
	/// Load a scene and replace the current one.
	/// </summary>
	/// <param name="absolutePath"></param>
	static void LoadScene(const std::string& absolutePath);
	/// <summary>
	/// Load a scene and add it to the current scene as a child of the specified entity.
	/// </summary>
	/// <param name="absolutePath"></param>
	static void LoadSubScene(const std::string& absolutePath, const std::shared_ptr<Entity>& parent = s_CurrentScene->GetRoot());
	static std::shared_ptr<Entity> CreateEntityAtRoot(const std::string& name) { return s_CurrentScene->AddEntityToRoot(name); }
};
