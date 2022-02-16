#include "Entity.h"

#include "../scene/SceneManager.h"
#include "../core/Log.h"
#include "ECS.h"

/* -- PUBLIC -- */

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

int Entity::FindNearestParentTransformIndex()
{
	if (m_Parent == nullptr)
	{
		return -1;
	}

	if (m_Parent->HasTransform())
	{
		return m_Parent->GetTransformIndex();
	}

	else
	{
		return m_Parent->FindNearestParentTransformIndex();
	}
}


/* ADD */

void Entity::AddTransformComponent(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	m_TransformComponent = ECS::CreateComponent<TransformComponent>(this, FindNearestParentTransformIndex(), translation, rotation, scale);
}

void Entity::AddMeshComponent(const VertexArrayAsset& vertexArray, const ShaderAsset& shader, const Material& material)
{
	m_MeshComponent = ECS::CreateComponent<MeshComponent>(this, vertexArray, shader, material);
}

void Entity::AddCameraComponent(const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
{
	m_CameraComponent = ECS::CreateComponent<CameraComponent>(this, fov, aspectRatio, nearPlane, farPlane);
}


/* REMOVE */

void Entity::RemoveTransformComponent()
{
	EntityModifier modifier = ECS::RemoveTransformComponent(m_TransformComponent);
	m_TransformComponent = NO_COMPONENT;

	if (modifier.EntityToModify != nullptr)
	{
		modifier.EntityToModify->SetTransformIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}

void Entity::RemoveMeshComponent()
{
	EntityModifier modifier = ECS::RemoveMeshComponent(m_MeshComponent);
	m_MeshComponent = NO_COMPONENT;

	if (modifier.EntityToModify != nullptr)
	{
		modifier.EntityToModify->SetMeshIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}

void Entity::RemoveCameraComponent()
{
	EntityModifier modifier = ECS::RemoveCameraComponent(m_CameraComponent);
	m_CameraComponent = NO_COMPONENT;

	if (modifier.EntityToModify != nullptr)
	{
		modifier.EntityToModify->SetCameraIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}


/* GET */

TransformComponent& Entity::GetTransformComponent()
{
	if (m_TransformComponent != NO_COMPONENT)
	{
		return ECS::GetTransformComponent(m_TransformComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a transform component!");
}

MeshComponent& Entity::GetMeshComponent()
{
	if (m_MeshComponent != NO_COMPONENT)
	{
		return ECS::GetMeshComponent(m_MeshComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a mesh component!");
}

CameraComponent& Entity::GetCameraComponent()
{
	if (m_CameraComponent != NO_COMPONENT)
	{
		return ECS::GetCameraComponent(m_CameraComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a camera component!");
}
