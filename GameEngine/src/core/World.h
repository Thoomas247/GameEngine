#pragma once

#include <map>
#include <string>
#include <memory>

#include "GameObject.h"

namespace World
{
	extern std::map<std::string, std::shared_ptr<GameObject>> GameObjects;

	void SetUp();
	void Update(const float& deltaTime);

	void AddGameObject(const std::string& name, const std::shared_ptr<GameObject>& object);
	std::shared_ptr<GameObject> GetGameObject(const std::string& path);
}