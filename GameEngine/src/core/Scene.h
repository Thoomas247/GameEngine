#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Entity.h"
#include "../components/TransformComponent.h"
#include "../components/MeshComponent.h"

class Scene
{
private:
	std::string m_Name = "";
	std::string m_Path = "";

	std::vector<Entity> m_Entities;	// defines the scene hierarchy

	std::vector<TransformComponent> m_TransformComponents;
	std::vector<MeshComponent> m_MeshComponents;

public:
	Scene(const std::string& name, const std::string& path);
	Scene(const Scene& scene);

	void Update(const float& deltaTime);

	std::vector<MeshComponent> GetMeshComponents() { return m_MeshComponents; }

	void CreateEntity(const std::string& name, Entity* parent = nullptr);
	Entity* GetEntity(const uint64_t& entityID);

	void CreateTransformComponent(const uint64_t& entityID, const glm::vec3& translation = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));
	TransformComponent* GetTransformComponent(const uint64_t& entityID);

	void CreateMeshComponent(const uint64_t& entityID, const VertexArray& vertexArray, const Shader& shader, const Material& material);
	MeshComponent* GetMeshComponent(const uint64_t& entityID);

private:
	void updateTransformComponents();
	void updateGlobalTransforms(const Entity& entity, const glm::mat4& parentTransform);

	void updateMeshComponents();
};