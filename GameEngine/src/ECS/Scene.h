#pragma once

#include <unordered_map>
#include <string>

#include "Entity.h"

class Scene
{
private:
	std::unordered_map<uint64_t, Entity> m_Entities;

public:
	uint64_t AddEntity(const std::string& name);
	void RemoveEntity(const uint64_t& entityID);
	Entity* GetEntity(const uint64_t& entityID);
};