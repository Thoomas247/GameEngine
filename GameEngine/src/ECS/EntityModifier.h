#pragma once

/* Forward declaration */
class Entity;

struct EntityModifier
{
	Entity* EntityToModify;
	int NewComponentIndex;

	EntityModifier(Entity* entity = nullptr, const int& newComponentIndex = 0)
	{
		EntityToModify = entity;
		NewComponentIndex = newComponentIndex;
	}
};