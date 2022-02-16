#pragma once

#include "Entity.h"

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