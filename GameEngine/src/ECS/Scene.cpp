#include "Scene.h"

#include "../core/Log.h"


/* -- PUBLIC -- */

uint64_t Scene::AddEntity(const std::string& name)
{
	Entity entity = Entity(name);
	m_Entities[entity.GetUUID()] = entity;
	return entity.GetUUID();
}

void Scene::RemoveEntity(const uint64_t& entityID)
{
	m_Entities.erase(entityID);
}

Entity* Scene::GetEntity(const uint64_t& entityID)
{
	auto it = m_Entities.find(entityID);

	if (it != m_Entities.end())
	{
		return &it->second;
	}

	LOG_ERROR("SCENE::Could not find entity with ID " + std::to_string(entityID) + "!");
}
