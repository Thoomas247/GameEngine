#pragma once

#include "../Entity.h"

class Component
{
private:
	Entity* m_Entity;

public:
	Component(Entity* entity)
	{
		m_Entity = entity;
	}

	Entity* GetEntity() { return m_Entity; }
};
