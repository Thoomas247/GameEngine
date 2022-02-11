#pragma once

#include <vector>
#include <unordered_map>

#include "Entity.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"

class ECS
{
private:
	static std::unordered_map<uint64_t, Entity> s_Entities;

	static std::vector<TransformComponent> s_TransformComponents;
	static std::vector<MeshComponent> s_MeshComponents;
	static std::vector<CameraComponent> s_CameraComponents;

public:
	static std::vector<TransformComponent> GetTransformComponents() { return s_TransformComponents; }
	static std::vector<MeshComponent> GetMeshComponents() { return s_MeshComponents; }
	static std::vector<CameraComponent> GetCameraComponents() { return s_CameraComponents; }

	// ENTITY
	static uint64_t CreateEntity(const std::string& name);
	static void RemoveEntity(const uint64_t& entityID);
	static Entity* GetEntity(const uint64_t& entityID);
	// TRANSFORM
	static void CreateTransformComponent(const uint64_t& entityID, const glm::vec3& translation = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));
	static void RemoveTransformComponent(const uint64_t& entityID);
	static TransformComponent* GetTransformComponent(const uint64_t& entityID);
	// MESH
	static void CreateMeshComponent(const uint64_t& entityID, const VertexArray& vertexArray = VertexArray(), const Shader& shader = Shader(), const Material& material = Material());
	static void RemoveMeshComponent(const uint64_t& entityID);
	static MeshComponent* GetMeshComponent(const uint64_t& entityID);
	// CAMERA
	static void CreateCameraComponent(const uint64_t& entityID, const float& fov = 75.0f, const float& aspectRatio = 16 / 9, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);
	static void RemoveCameraComponent(const uint64_t& entityID);
	static CameraComponent* GetCameraComponent(const uint64_t& entityID);
};

