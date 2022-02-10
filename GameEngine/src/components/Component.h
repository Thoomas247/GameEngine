#pragma once

#include "../core/Entity.h"

class Component
{
private:
	uint64_t m_EntityID;

public:
	Component(const uint64_t& entityID)
	{
		m_EntityID = entityID;
	}

	const uint64_t GetEntityID() const { return m_EntityID; }
};

