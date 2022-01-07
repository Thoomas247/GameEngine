#include "World.h"

std::map<std::string, std::shared_ptr<GameObject>> World::GameObjects;

void World::SetUp()
{
	for (auto& [name, object] : GameObjects)
	{
		object->SetUp();
	}
}

void World::Update(const float& deltaTime)
{
	for (auto& [name, object] : GameObjects)
	{
		object->Update(deltaTime);
	}
}

void World::AddGameObject(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	GameObjects[name] = object;
}

std::shared_ptr<GameObject> World::GetGameObject(const std::string& path)
{
	size_t index = path.find_first_of("/");
	std::string name = path.substr(0, index);

	const std::map<std::string, std::shared_ptr<GameObject>>::iterator it = GameObjects.find(name);
	if (it != GameObjects.end())
	{
		if (index == std::string::npos)
		{
			return it->second;
		}

		std::string newPath = path.substr(index);
		return it->second->GetChild(newPath);
	}

	return nullptr;
}