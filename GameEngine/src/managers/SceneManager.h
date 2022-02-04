#pragma once

#include <vector>
#include <memory>

#include "../core/Scene.h"

/// <summary>
/// Class to manipulate the current scene.
/// </summary>
class SceneManager
{
private:
	static Scene* s_CurrentScene;

public:
	static void SetScene(Scene* scene) { s_CurrentScene = scene; }

	static const std::vector<std::shared_ptr<GameObject>>* GetSceneGameObjects() { return s_CurrentScene->GetGameObjects(); }

	static void SetUpScene() { s_CurrentScene->SetUp(); }
	static void UpdateScene(const float& deltaTime) { s_CurrentScene->Update(deltaTime); }
	static void AddGameObjectToScene(const std::shared_ptr<GameObject>& gameObject) { s_CurrentScene->AddGameObject(gameObject); }
	static void SaveScene() { s_CurrentScene->Save(); }
	static void SaveSceneAs(const std::string& path, const std::string& name) { s_CurrentScene->SaveAs(path, name); }
};

