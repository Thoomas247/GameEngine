#include "Entity.h"

#include "../scene/SceneManager.h"
#include "../core/Log.h"

#include "components/TransformComponent.h"


int ComponentTypeID::m_Counter = 0;


/* -- PUBLIC -- */

Entity* Entity::GetParent()
{
	if (m_Parent == nullptr) 
	{ 
		LOG_ERROR("ENTITY::Parent is nullptr!"); 
	} 
	return m_Parent;
}

std::shared_ptr<Entity> Entity::CreateChild(const std::string& name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name, this);
	m_ChildrenEntities[entity->GetUUID()] = entity;
	return entity;
}

void Entity::AddChild(const std::shared_ptr<Entity>& entity)
{
	entity->SetParent(this);
	m_ChildrenEntities[entity->GetUUID()] = entity;
}

void Entity::RemoveChild(const uint64_t& entityID)
{
	m_ChildrenEntities.erase(entityID);
}

int Entity::FindTransformParent()
{
	if (m_Parent == nullptr)
	{
		return -1;
	}

	if (m_Parent->HasComponent<TransformComponent>())
	{
		return m_Parent->GetComponentIndex<TransformComponent>();
	}

	else
	{
		return m_Parent->FindTransformParent();
	}
}


/* -- PRIVATE -- */

void Entity::setChildrenTransformParents(const int& index)
{
	for (auto& [key, child] : m_ChildrenEntities)
	{
		child->GetComponent<TransformComponent>().SetParentIndex(index);
	}
}
