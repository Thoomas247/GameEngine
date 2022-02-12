#pragma once

//#include "../Entity.h"
class Entity;

class Component
{
private:
	Entity* m_Entity;

public:
	Component(Entity* entity)
	{
		m_Entity = entity;
	}

	Entity* GetEntity() const { return m_Entity; }
};
