#pragma once

#include <map>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../core/UUID.h"
#include "../graphics/GraphicsAssetManager.h"

#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/CameraComponent.h"

constexpr int NO_COMPONENT = -1;

class Entity
{
private:
	uint64_t m_UUID;
	std::string m_Name;
	std::map<uint64_t, std::shared_ptr<Entity>> m_ChildrenEntities;

	int m_TransformComponent = NO_COMPONENT;
	int m_MeshComponent = NO_COMPONENT;
	int m_CameraComponent = NO_COMPONENT;

public:
	Entity()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "Empty Entity";
	}

	Entity(const std::string& name)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string& GetName() const { return m_Name; }
	const std::map<uint64_t, std::shared_ptr<Entity>>& GetChildren() const { return m_ChildrenEntities; }

	std::shared_ptr<Entity> CreateChild(const std::string& name);
	void AddChild(const std::shared_ptr<Entity>& entity);
	void RemoveChild(const uint64_t& entityID);

	/* COMPONENT FUNCTIONS */

	// HAS COMPONENT
	bool HasTransform() { return m_TransformComponent != NO_COMPONENT; }
	bool HasMesh() { return m_MeshComponent != NO_COMPONENT; }
	bool HasCamera() { return m_CameraComponent != NO_COMPONENT; }

	// SET INDEX
	void SetTransformIndex(const int& index) { m_TransformComponent = index; }
	void SetMeshIndex(const int& index) { m_MeshComponent = index; }
	void SetCameraIndex(const int& index) { m_CameraComponent = index; }

	// ADD
	void AddTransformComponent(const glm::vec3& translation = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));
	void AddMeshComponent(const VertexArrayAsset& vertexArray = GraphicsAssetManager::LoadVertexArray(), const ShaderAsset& shader = GraphicsAssetManager::LoadShader(), const Material& material = Material());
	void AddCameraComponent(const float& fov = 75.0f, const float& aspectRatio = 16 / 9, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);

	// REMOVE
	void RemoveTransformComponent();
	void RemoveMeshComponent();
	void RemoveCameraComponent();

	// GET
	TransformComponent& GetTransformComponent();
	MeshComponent& GetMeshComponent();
	CameraComponent& GetCameraComponent();
};
