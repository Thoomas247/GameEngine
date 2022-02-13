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

	std::shared_ptr<Entity> m_RootEntity;	// the scene requires a root entity to define the hierarchy
	std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_Entities;

public:
	Scene()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "New Scene";

		m_RootEntity = std::make_shared<Entity>("Root");
		m_Entities[m_RootEntity->GetUUID()] = m_RootEntity;
	}

	Scene(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		m_RootEntity = std::make_shared<Entity>("Root");
		m_Entities[m_RootEntity->GetUUID()] = m_RootEntity;
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }

	void Update(const float& deltaTime);

	std::shared_ptr<Entity> AddEntity(const std::string& name, const uint64_t& parentID = 0);
	void RemoveEntity(const uint64_t& entityID);
	std::shared_ptr<Entity> GetEntity(const uint64_t& entityID);

private:
	void updateTransformComponents(const std::shared_ptr<Entity>&, const glm::mat4& parentMat);
};