#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "../core/UUID.h"
#include "../ECS/Entity.h"

class Scene
{
private:
	uint64_t m_UUID;
	std::string m_Name;

	std::shared_ptr<Entity> m_RootEntity;

public:
	Scene()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "New Scene";

		m_RootEntity = std::make_shared<Entity>("Root");
	}

	Scene(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		m_RootEntity = std::make_shared<Entity>("Root");
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }
	std::shared_ptr<Entity> GetRoot() { return m_RootEntity; }

	void Update(const float& deltaTime);

	std::shared_ptr<Entity> AddEntityToRoot(const std::string& name);
};