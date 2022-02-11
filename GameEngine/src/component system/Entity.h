#pragma once

#include <vector>
#include <string>

#include "../core/UUID.h"

#define NO_COMPONENT -1

/// <summary>
/// Contains the indices of the components associated with an Entity.
/// Exists so that the Entity class itself doesn't get too overcrowded.
/// </summary>
struct ComponentIndexContainer
{
	int Transform = NO_COMPONENT;
	int Mesh = NO_COMPONENT;
	int Camera = NO_COMPONENT;
};

class Entity
{
public:
	ComponentIndexContainer ComponentIndices;

private:
	uint64_t m_UUID;
	std::string m_Name;

	std::vector<uint64_t> m_ChildrenEntityIDs;

public:
	Entity() 
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "Empty Entity";

		ComponentIndices = ComponentIndexContainer();
	}

	Entity(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		ComponentIndices = ComponentIndexContainer();
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string GetName() const { return m_Name; }

	const std::vector<uint64_t> GetChildren() const { return m_ChildrenEntityIDs; }
	void AddChild(const uint64_t& entityID) { m_ChildrenEntityIDs.push_back(entityID); }
};
