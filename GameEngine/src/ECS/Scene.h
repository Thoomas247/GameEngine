#pragma once

#include <unordered_map>
#include <string>

#include "../core/UUID.h"
#include "Entity.h"

class Scene
{
private:
	uint64_t m_UUID;
	std::string m_Name;

	Entity* m_RootEntity;	// the scene requires a root entity to define the hierarchy
	std::unordered_map<uint64_t, Entity> m_Entities;

public:
	Scene()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "New Scene";

		Entity root = Entity("Root");
		m_Entities[root.GetUUID()] = root;
		m_RootEntity = &m_Entities[root.GetUUID()];
	}

	Scene(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		Entity root = Entity("Root");
		m_Entities[root.GetUUID()] = root;
		m_RootEntity = &m_Entities[root.GetUUID()];
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }

	void Update(const float& deltaTime);

	Entity* AddEntity(const std::string& name);
	void RemoveEntity(const uint64_t& entityID);
	Entity* GetEntity(const uint64_t& entityID);

private:
	void updateTransformComponents(Entity* entity, const glm::mat4& parentMat);
};