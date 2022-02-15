#include "Scene.h"

#include "../core/Log.h"
#include "../ECS/ECS.h"

/* -- PUBLIC -- */

void Scene::Update(const float& deltaTime)
{
	// hierarchy dependent update
	updateTransformComponents(m_RootEntity, glm::mat4(1.0f));


}

std::shared_ptr<Entity> Scene::AddEntity(const std::string& name, const uint64_t& parentID)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

	uint64_t entityID = entity->GetUUID();

	m_Entities[entityID] = entity;

	if (parentID == 0)
	{
		m_RootEntity->AddChild(entityID);
	}
	else
	{
		GetEntity(parentID)->AddChild(entityID);
	}

	return m_Entities[entityID];
}

void Scene::RemoveEntity(const uint64_t& entityID)
{
	m_Entities.erase(entityID);
}

std::shared_ptr<Entity> Scene::GetEntity(const uint64_t& entityID)
{
	auto it = m_Entities.find(entityID);

	if (it != m_Entities.end())
	{
		return it->second;
	}

	LOG_ERROR("SCENE::Could not find entity with ID " + std::to_string(entityID) + "!");
}

/* -- PRIVATE -- */

void Scene::updateTransformComponents(const std::shared_ptr<Entity>& entity, const glm::mat4& parentMat)
{
	if (entity->HasTransform())
	{
		const glm::mat4& transform = entity->GetTransformComponent()->UpdateTransforms(parentMat);
		for (const auto& childID : entity->GetChildren())
		{
			updateTransformComponents(GetEntity(childID), transform);
		}

		if (entity->HasMesh())
		{
			entity->GetMeshComponent()->SetModelMat(transform);
		}
	}
	else
	{
		for (const auto& childID : entity->GetChildren())
		{
			updateTransformComponents(GetEntity(childID), parentMat);
		}
	}
}

