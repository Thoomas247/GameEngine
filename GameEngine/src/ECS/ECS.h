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

	/// <summary>
	/// Overloaded AddComponent method to determine in which vector to add the component depending on its type
	/// </summary>
	/// <param name="component">The component to add to the vector</param>
	/// <returns>The 0 based index of the component that was added</returns>
	static int AddComponent(const TransformComponent& component) { s_TransformComponents.push_back(component); return (int)s_TransformComponents.size() - 1; }
	static int AddComponent(const MeshComponent& component) { s_MeshComponents.push_back(component); return (int)s_MeshComponents.size() - 1; }
	static int AddComponent(const CameraComponent& component) { s_CameraComponents.push_back(component);  return (int)s_CameraComponents.size() - 1; }

public:

	/* Component Functions */

	static std::vector<TransformComponent>& GetTransformComponents() { return s_TransformComponents; }
	static std::vector<MeshComponent>& GetMeshComponents() { return s_MeshComponents; }
	static std::vector<CameraComponent>& GetCameraComponents() { return s_CameraComponents; }

	static size_t GetNumTransforms() { return s_TransformComponents.size(); }
	static size_t GetNumMeshes() { return s_MeshComponents.size(); }
	static size_t GetNumCameras() { return s_CameraComponents.size(); }

	// CREATE
	
	/// <summary>
	/// Fancier way of creating components using templates
	/// This might be a bit complicated because the arguments are not the same for all component types
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh or Camera)</typeparam>
	/// <typeparam name="...P">Variable list of parameters to pass to constructor</typeparam>
	/// <returns></returns>
	template <class T, typename... P>
	static int CreateComponent(P... params)
	{
		return AddComponent(T(params...));
	}

	/* Old versions, not using templates
	static int CreateTransformComponent(Entity* entity, const int& parentIndex, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale);
	static int CreateMeshComponent(Entity* entity, const VertexArrayAsset& vertexArray, const ShaderAsset& shader, const Material& material);
	static int CreateCameraComponent(Entity* entity, const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane);
	*/

	// REMOVE
	static EntityModifier RemoveTransformComponent(const int& index);
	static EntityModifier RemoveMeshComponent(const int& index);
	static EntityModifier RemoveCameraComponent(const int& index);

	// GET
	static TransformComponent& GetTransformComponent(const int& index);
	static MeshComponent& GetMeshComponent(const int& index);
	static CameraComponent& GetCameraComponent(const int& index);
};
