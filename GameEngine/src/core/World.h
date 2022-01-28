#pragma once

#include <map>
#include <string>
#include <memory>

#include "GameObject.h"

class World
{
public:
	static std::map<std::string, std::shared_ptr<GameObject>> s_GameObjects;

public:
	static void SetUp();
	static void Update(const float& deltaTime);

	std::shared_ptr<GameObject> GetGameObject(const std::string& path);
	static void AddGameObject(const std::string& name, const std::shared_ptr<GameObject>& object);
};