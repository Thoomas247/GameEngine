#include "Scene.h"

#include "Log.h"

// PUBLIC
Scene::Scene(const std::string& name, const std::string& path)
{
	m_Name = name;
	m_Path = path;
}

Scene::Scene(const Scene& scene)
{

}

/// <summary>
/// Adds the given game object to this scene.
/// </summary>
/// <param name="gameObject"></param>
void Scene::AddGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	m_GameObjects.push_back(gameObject);
}

/// <summary>
/// Save this scene using the current path/name.
/// </summary>
/// <returns></returns>
bool Scene::Save()
{
	return saveToFile();
}

/// <summary>
/// Save this scene using a new path/name.
/// </summary>
/// <param name="path"></param>
/// <param name="name"></param>
/// <returns></returns>
bool Scene::SaveAs(const std::string& path, const std::string& name)
{
	m_Path = path;
	m_Name = name;
	return saveToFile();
}

/// <summary>
/// Runs all the SetUp() functions of the GameObjects in this scene.
/// </summary>
void Scene::SetUp()
{
	for (const auto& object : m_GameObjects)
	{
		object->SetUp();
	}
}

/// <summary>
/// Responsible for updating the scene (game world) every frame.
/// Runs all the Update() functions of the GameObjects in this scene.
/// </summary>
/// <param name="deltaTime"></param>
void Scene::Update(const float& deltaTime)
{
	for (const auto& object : m_GameObjects)
	{
		object->Update(deltaTime);
	}
}

// PRIVATE
bool Scene::saveToFile()
{
	if (m_Path == "" || m_Name == "")
	{
		LOG_ERROR("SCENE::Scene could not be saved due to missing path or name!")
		return false;
	}
	else 
	{

	}
}
