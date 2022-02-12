#include "Scene.h"

#include "../core/Log.h"


/* -- PUBLIC -- */

void Scene::Update(const float& deltaTime)
{
	// hierarchy dependent update
	updateTransformComponents(m_RootEntity, glm::mat4(1.0f));


}

Entity* Scene::AddEntity(const std::string& name)
{
	return m_RootEntity->CreateChild(name);
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


/* -- PRIVATE -- */

void Scene::updateTransformComponents(Entity* entity, const glm::mat4& parentMat)
{
	if (entity->HasTransform())
	{
		const glm::mat4& transform = entity->GetTransformComponent()->UpdateTransforms(parentMat);
		for (const uint64_t& childID : entity->GetChildrenIDs())
		{
			updateTransformComponents(GetEntity(childID), transform);
		}
	}
	else
	{
		for (const uint64_t& childID : entity->GetChildrenIDs())
		{
			updateTransformComponents(GetEntity(childID), parentMat);
		}
	}
}
