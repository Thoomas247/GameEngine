#include "Scene.h"

#include "json/json.hpp"
using json = nlohmann::json;

#include "Log.h"

// PUBLIC

Scene::Scene(const std::string& name, const std::string& path)
{
	m_Name = name;
	m_Path = path;
}

Scene::Scene(const Scene& scene)
{
	// TODO: implement copy constructor
}


void Scene::Update(const float& deltaTime)
{
	updateTransformComponents();
}


/// <summary>
/// Creates a new entity in this scene with the specified name and parent (if specified).
/// </summary>
/// <param name="name"></param>
/// <param name="parentID"></param>
void Scene::CreateEntity(const std::string& name, Entity* parent)
{
	if (parent == nullptr)
		m_Entities.push_back(Entity(name));

	else
		parent->AddChild(Entity(name));
}

/// <summary>
/// Currently a simple search. 
/// Returns a pointer to an entity.
/// TODO: store entities in a binary tree.
/// </summary>
/// <param name="ID"></param>
/// <returns></returns>
Entity* Scene::GetEntity(const uint64_t& entityID)
{
	return nullptr;
}


/// <summary>
/// Creates and adds a transform component to the specified entity.
/// Assumes that the entity exists (for now).
/// </summary>
/// <param name="entityID"></param>
/// <param name="translation"></param>
/// <param name="rotation"></param>
/// <param name="scale"></param>
void Scene::CreateTransformComponent(const uint64_t& entityID, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	m_TransformComponents.push_back(TransformComponent(entityID, translation, rotation, scale));
}

/// <summary>
/// Returns a pointer to a transform component.
/// </summary>
/// <param name="entityID"></param>
/// <returns></returns>
TransformComponent* Scene::GetTransformComponent(const uint64_t& entityID)
{
	for (unsigned int i = 0; i < m_TransformComponents.size(); i++)
	{
		TransformComponent* comp = &m_TransformComponents[i];
		if (comp->GetEntityID() == entityID)
		{
			return comp;
		}
	}

	LOG_ERROR("SCENE::Could not find a transform component associated with the entity ID " + std::to_string(entityID) + "!");
	// should assert here
	// TODO: add assertions to engine
}


/// <summary>
/// Creates and adds a mesh component to the specified entity.
/// Assumes the entity exists (for now).
/// </summary>
/// <param name="entityID"></param>
/// <param name="vertexArray"></param>
/// <param name="shader"></param>
/// <param name="material"></param>
void Scene::CreateMeshComponent(const uint64_t& entityID, const VertexArray& vertexArray, const Shader& shader, const Material& material)
{
	m_MeshComponents.push_back(MeshComponent(entityID, vertexArray, shader, material));
}

/// <summary>
/// Returns a pointer to a mesh component.
/// </summary>
/// <param name="entityID"></param>
/// <returns></returns>
MeshComponent* Scene::GetMeshComponent(const uint64_t& entityID)
{
	for (unsigned int i = 0; i < m_MeshComponents.size(); i++)
	{
		MeshComponent* comp = &m_MeshComponents[i];
		if (comp->GetEntityID() == entityID)
		{
			return comp;
		}
	}

	LOG_ERROR("SCENE::Could not find a mesh component associated with the entity ID " + std::to_string(entityID) + "!");
	// should assert here
	// TODO: add assertions to engine
}


// PRIVATE

/// <summary>
/// Updates (recalculates) all the transform components in the scene.
/// </summary>
void Scene::updateTransformComponents()
{
	// local transform update
	for (TransformComponent& component : m_TransformComponents)
	{
		if (component.HasChanged())
			component.UpdateLocalTransform();
	}

	// transform update (hierarchy dependent)
	glm::mat4 identityMat = glm::mat4(1.0f);
	for (const Entity& entity : m_Entities)
	{
		updateGlobalTransforms(entity, identityMat);
	}
}

/// <summary>
/// Recursive function used by Scene::updateTransformComponents().
/// </summary>
/// <param name="entity"></param>
/// <param name="parentTransform"></param>
void Scene::updateGlobalTransforms(const Entity& entity, const glm::mat4& parentTransform)
{
	const glm::mat4& mat = GetTransformComponent(entity.GetUUID())->UpdateGlobalTransform(parentTransform);

	for (const Entity& ent : entity.GetChildren())
	{
		updateGlobalTransforms(ent, mat);
	}
}


/// <summary>
/// Updates all of the mesh components in this scene.
/// </summary>
void Scene::updateMeshComponents()
{
	
}
