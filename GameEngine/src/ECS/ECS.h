#pragma once

#include <vector>

#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/CameraComponent.h"

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

class ECS
{
private:
	static std::vector<TransformComponent> s_TransformComponents;
	static std::vector<MeshComponent> s_MeshComponents;
	static std::vector<CameraComponent> s_CameraComponents;

public:
	static std::vector<TransformComponent> GetTransformComponents() { return s_TransformComponents; }
	static std::vector<MeshComponent> GetMeshComponents() { return s_MeshComponents; }
	static std::vector<CameraComponent> GetCameraComponents() { return s_CameraComponents; }

	// CREATE
	static int CreateTransformComponent(Entity* entity, const glm::vec3& translation = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));
	static int CreateMeshComponent(Entity* entity, const VertexArray& vertexArray = VertexArray(), const Shader& shader = Shader(), const Material& material = Material());
	static int CreateCameraComponent(Entity* entity, const float& fov = 75.0f, const float& aspectRatio = 16 / 9, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);
	
	// REMOVE
	static EntityModifier RemoveTransformComponent(const int& index);
	static EntityModifier RemoveMeshComponent(const int& index);
	static EntityModifier RemoveCameraComponent(const int& index);

	// GET
	static TransformComponent* GetTransformComponent(const int& index);
	static MeshComponent* GetMeshComponent(const int& index);
	static CameraComponent* GetCameraComponent(const int& index);
};

