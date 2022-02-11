#include "ECS.h"

#include "../core/Log.h"


std::unordered_map<uint64_t, Entity> ECS::s_Entities;

std::vector<TransformComponent> ECS::s_TransformComponents;
std::vector<MeshComponent> ECS::s_MeshComponents;
std::vector<CameraComponent> ECS::s_CameraComponents;


/* ENTITY FUNCTIONS */

uint64_t ECS::CreateEntity(const std::string& name)
{
	Entity entity = Entity(name);
	uint64_t entityID = entity.GetUUID();

	s_Entities[entityID] = entity;

	return entityID;
}

void ECS::RemoveEntity(const uint64_t& entityID)
{
	ComponentIndexContainer container = GetEntity(entityID)->ComponentIndices;

	if (container.Transform != NO_COMPONENT) RemoveTransformComponent(entityID);
	if (container.Mesh != NO_COMPONENT) RemoveMeshComponent(entityID);
	if (container.Camera != NO_COMPONENT) RemoveCameraComponent(entityID);

	s_Entities.erase(entityID);
}

Entity* ECS::GetEntity(const uint64_t& entityID)
{
	auto it = s_Entities.find(entityID);
	if (it != s_Entities.end())
	{
		return &it->second;
	}

	LOG_ERROR("ECS::Could not find an entity with ID " + std::to_string(entityID) + "!");
}


/* TRANSFORM COMPONENT FUNCTIONS */

void ECS::CreateTransformComponent(const uint64_t& entityID, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	int index = (int)s_TransformComponents.size();
	s_TransformComponents.push_back(TransformComponent(entityID, translation, rotation, scale));
	GetEntity(entityID)->ComponentIndices.Transform = index;
}

void ECS::RemoveTransformComponent(const uint64_t& entityID)
{
	Entity* entity = GetEntity(entityID);
	int index = entity->ComponentIndices.Transform;
	entity->ComponentIndices.Transform = NO_COMPONENT;

	if (index < 0 || index >= s_TransformComponents.size())
	{
		LOG_ERROR("ECS::Could not delete the transform component associated with the entity ID " + std::to_string(entityID) + "!");
		// should throw exception here
	}

	else if (index == s_TransformComponents.size() - 1)
	{
		s_TransformComponents.erase(s_TransformComponents.end() - 1);
	}

	else
	{
		s_TransformComponents[index] = s_TransformComponents.back();	// replace transform to delete by last transform
		s_TransformComponents.erase(s_TransformComponents.end() - 1);	// delete last transform

		Entity* entityToUpdate = GetEntity(s_TransformComponents[index].GetEntityID());
		entityToUpdate->ComponentIndices.Transform = index;	// update the entity whose tranform changed location
	}
}

TransformComponent* ECS::GetTransformComponent(const uint64_t& entityID)
{
	int transformIndex = GetEntity(entityID)->ComponentIndices.Transform;

	if (transformIndex > 0 && transformIndex < s_TransformComponents.size())
	{
		return &s_TransformComponents[transformIndex];
	}

	LOG_ERROR("ECS::Could not find a transform component associated with the entity ID " + std::to_string(entityID) + "!");
}


/* MESH COMPONENT FUNCTIONS */

void ECS::CreateMeshComponent(const uint64_t& entityID, const VertexArray& vertexArray, const Shader& shader, const Material& material)
{
	int index = (int)s_MeshComponents.size();
	s_MeshComponents.push_back(MeshComponent(entityID, vertexArray, shader, material));
	GetEntity(entityID)->ComponentIndices.Mesh = index;
}

void ECS::RemoveMeshComponent(const uint64_t& entityID)
{
	Entity* entity = GetEntity(entityID);
	int index = entity->ComponentIndices.Mesh;
	entity->ComponentIndices.Mesh = NO_COMPONENT;

	if (index < 0 || index >= s_MeshComponents.size())
	{
		LOG_ERROR("ECS::Could not delete the mesh component associated with the entity ID " + std::to_string(entityID) + "!");
	}

	else if (index == s_MeshComponents.size() - 1)
	{
		s_MeshComponents.erase(s_MeshComponents.end() - 1);
	}

	else
	{
		s_MeshComponents[index] = s_MeshComponents.back();	// replace mesh to delete by last mesh
		s_MeshComponents.erase(s_MeshComponents.end() - 1);	// delete last mesh

		Entity* entityToUpdate = GetEntity(s_MeshComponents[index].GetEntityID());
		entityToUpdate->ComponentIndices.Mesh = index;	// update the entity whose mesh changed location
	}
}

MeshComponent* ECS::GetMeshComponent(const uint64_t& entityID)
{
	int meshIndex = GetEntity(entityID)->ComponentIndices.Mesh;

	if (meshIndex > 0 && meshIndex < s_MeshComponents.size())
	{
		return &s_MeshComponents[meshIndex];
	}

	LOG_ERROR("ECS::Could not find a mesh component associated with the entity ID " + std::to_string(entityID) + "!");
	// should throw exception here
}


/* CAMERA COMPONENT FUNCTIONS */

void ECS::CreateCameraComponent(const uint64_t& entityID, const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
{
	int index = (int)s_TransformComponents.size();
	s_CameraComponents.push_back(CameraComponent(entityID, fov, aspectRatio, nearPlane, farPlane));
	GetEntity(entityID)->ComponentIndices.Camera = index;
}

void ECS::RemoveCameraComponent(const uint64_t& entityID)
{
	Entity* entity = GetEntity(entityID);
	int index = entity->ComponentIndices.Camera;
	entity->ComponentIndices.Camera = NO_COMPONENT;

	if (index < 0 || index >= s_CameraComponents.size())
	{
		LOG_ERROR("ECS::Could not delete the mesh component associated with the entity ID " + std::to_string(entityID) + "!");
	}

	else if (index == s_CameraComponents.size() - 1)
	{
		s_CameraComponents.erase(s_CameraComponents.end() - 1);
	}

	else
	{
		s_CameraComponents[index] = s_CameraComponents.back();
		s_CameraComponents.erase(s_CameraComponents.end() - 1);

		Entity* entityToUpdate = GetEntity(s_CameraComponents[index].GetEntityID());
		entityToUpdate->ComponentIndices.Camera = index;
	}
}

CameraComponent* ECS::GetCameraComponent(const uint64_t& entityID)
{
	int cameraIndex = GetEntity(entityID)->ComponentIndices.Camera;

	if (cameraIndex > 0 && cameraIndex < s_CameraComponents.size())
	{
		return &s_CameraComponents[cameraIndex];
	}

	LOG_ERROR("ECS::Could not find a camera component associated with the entity ID " + std::to_string(entityID) + "!");
}