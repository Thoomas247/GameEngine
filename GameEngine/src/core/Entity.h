#pragma once

#include <vector>
#include <string>

#include "UUID.h"

class Entity
{
private:
	uint64_t m_UUID;
	std::string m_Name;

	std::vector<Entity> m_ChildrenEntities;

public:
	Entity() 
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "Empty Entity";
	}

	Entity(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }

	const std::vector<Entity> GetChildren() const { return m_ChildrenEntities; }
	void AddChild(const Entity& entity) { m_ChildrenEntities.push_back(entity); }
};
