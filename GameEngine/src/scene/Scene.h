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

		m_RootEntity = std::make_shared<Entity>("Root", nullptr);
	}

	Scene(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		m_RootEntity = std::make_shared<Entity>("Root", nullptr);
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }
	std::shared_ptr<Entity> GetRoot() { return m_RootEntity; }

	/// <summary>
	/// Updates the scene to prepare it for the next frame.
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(const float& deltaTime);
	/// <summary>
	/// Creates and adds an entity to the scene root.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::shared_ptr<Entity> AddEntityToRoot(const std::string& name);
};