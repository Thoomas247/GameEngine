#include "Entity.h"

#include "../scene/SceneManager.h"
#include "../core/Log.h"

#include "components/TransformComponent.h" // for SetComponentIndex<TransformComponent> and RemoveComponent<TransformComponent> specializations

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

/// <summary>
/// Specialized function for the case where a TransformComponent has moved.
/// This is the only component which is hierarchy dependent, and therefore this entity's
/// children must be notified that the TransformComponent has moved.
/// </summary>
/// <param name="index">The new index of the TransformComponent</param>
template<>
void Entity::SetComponentIndex<TransformComponent>(const int& index)
{
	static int n = ComponentTypeID::Value<TransformComponent>();
	m_Components[n] = index;
	setChildrenTransformParents(index);
}

/// <summary>
/// Specialized function for the case where a TransformComponent has been removed.
/// This is the only component which is hierarchy dependent, and therefore this entity's
/// children must be notified that the TransformComponent has been removed.
/// </summary>
template <>
void Entity::RemoveComponent<TransformComponent>()
{
	static int n = ComponentTypeID::Value<TransformComponent>();
	EntityModifier modifier = ECS::RemoveComponent<TransformComponent>(m_Components[n]);
	m_Components[n] = -1;

	if (modifier.EntityToModify != nullptr)
	{
		modifier.EntityToModify->SetComponentIndex<TransformComponent>(modifier.NewComponentIndex);	// update the entity whose component was moved
	}

	setChildrenTransformParents(FindTransformParent());
}

/* -- PRIVATE -- */

void Entity::setChildrenTransformParents(const int& index)
{
	for (auto& [key, child] : m_ChildrenEntities)
	{
		child->GetComponent<TransformComponent>().SetParentIndex(index);
	}
}
