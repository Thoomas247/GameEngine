#include "World.h"

std::map<std::string, std::shared_ptr<GameObject>> World::s_GameObjects;

// PUBLIC

/// <summary>
/// Before the main loop runs, SetUp() must be called to run all the onSetUp() functions of the GameObjects in the current scene.
/// </summary>
void World::SetUp()
{
	for (auto& [name, object] : s_GameObjects)
	{
		object->SetUp();
	}
}

/// <summary>
/// Responsible for updating the game world every frame.
/// </summary>
/// <param name="deltaTime"></param>
void World::Update(const float& deltaTime)
{
	for (auto& [name, object] : s_GameObjects)
	{
		object->Update(deltaTime);
	}
}

/// <summary>
/// Add a GameObject to the root of the scene.
/// </summary>
/// <param name="name"></param>
/// <param name="object"></param>
void World::AddGameObject(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	s_GameObjects[name] = object;
}