#pragma once

#include <map>
#include <string>
#include <memory>

#include "GameObject.h"

class World
{
private:
	static std::map<std::string, std::shared_ptr<GameObject>> s_GameObjects;

public:
	static void SetUp();
	static void Update(const float& deltaTime);

	static void AddGameObject(const std::string& name, const std::shared_ptr<GameObject>& object);
	static std::shared_ptr<GameObject> GetGameObject(const std::string& path);

	static std::map<std::string, std::shared_ptr<GameObject>> GetGameObjects() { return s_GameObjects; }
};