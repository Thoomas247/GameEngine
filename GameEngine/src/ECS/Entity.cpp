#include "Entity.h"

#include "../managers/SceneManager.h"
#include "../core/Log.h"

/* -- PUBLIC -- */

void Entity::CreateChild(const std::string& name)
{
	m_ChildrenEntityIDs.push_back(SceneManager::AddEntity(name));
}

void Entity::AddChild(const uint64_t& entityID)
{
	m_ChildrenEntityIDs.push_back(entityID);
}

void Entity::RemoveChild(const uint64_t& entityID)
{
	for (unsigned int i = 0; i < m_ChildrenEntityIDs.size(); i++)
	{
		if (m_ChildrenEntityIDs[i] == entityID)
		{
			m_ChildrenEntityIDs.erase(m_ChildrenEntityIDs.begin() + i);
		}
	}
}


/* ADD */

void Entity::AddTransformComponent(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	m_TransformComponent = ECS::CreateTransformComponent(m_UUID, translation, rotation, scale);
}

void Entity::AddMeshComponent(const VertexArray& vertexArray, const Shader& shader, const Material& material)
{
	m_MeshComponent = ECS::CreateMeshComponent(m_UUID, vertexArray, shader, material);
}

void Entity::AddCameraComponent(const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
{
	m_CameraComponent = ECS::CreateCameraComponent(m_UUID, fov, aspectRatio, nearPlane, farPlane);
}


/* REMOVE */

void Entity::RemoveTransformComponent()
{
	EntityModifier modifier = ECS::RemoveTransformComponent(m_TransformComponent);
	m_TransformComponent = NO_COMPONENT;

	if (modifier.EntityID != 0)
	{
		SceneManager::GetEntity(modifier.EntityID)->SetTransformIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}

void Entity::RemoveMeshComponent()
{
	EntityModifier modifier = ECS::RemoveMeshComponent(m_MeshComponent);
	m_MeshComponent = NO_COMPONENT;

	if (modifier.EntityID != 0)
	{
		SceneManager::GetEntity(modifier.EntityID)->SetMeshIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}

void Entity::RemoveCameraComponent()
{
	EntityModifier modifier = ECS::RemoveCameraComponent(m_CameraComponent);
	m_CameraComponent = NO_COMPONENT;

	if (modifier.EntityID != 0)
	{
		SceneManager::GetEntity(modifier.EntityID)->SetCameraIndex(modifier.NewComponentIndex);	// update the entity whose component was moved
	}
}


/* GET */

TransformComponent* Entity::GetTransformComponent()
{
	if (m_TransformComponent != NO_COMPONENT)
	{
		return ECS::GetTransformComponent(m_TransformComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a transform component!");
}

MeshComponent* Entity::GetMeshComponent()
{
	if (m_MeshComponent != NO_COMPONENT)
	{
		return ECS::GetMeshComponent(m_MeshComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a mesh component!");
}

CameraComponent* Entity::GetCameraComponent()
{
	if (m_CameraComponent != NO_COMPONENT)
	{
		return ECS::GetCameraComponent(m_CameraComponent);
	}

	LOG_ERROR("ENTITY::Entity " + std::to_string(m_UUID) + " does not have a camera component!");
}
