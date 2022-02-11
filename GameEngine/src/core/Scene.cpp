#include "Scene.h"

#include "Log.h"


/* -- PUBLIC -- */

void Scene::Update(const float& deltaTime)
{
	updateTransformComponents(ECS::GetEntity(m_RootID), glm::mat4(1.0f));
}


/* -- PRIVATE -- */

/// <summary>
/// Updates (recalculates) all the transform components in the scene.
/// </summary>
void Scene::updateTransformComponents(Entity* entity, const glm::mat4& parentTransform)
{
	glm::mat4 matrix = glm::mat4(1.0f);

	if (entity->ComponentIndices.Transform >= 0)
	{
		TransformComponent* comp = ECS::GetTransformComponent(entity->GetUUID());

		comp->UpdateTransforms(parentTransform);
		matrix = comp->GetGlobalTransform();
	}

	for (const uint64_t& ID : entity->GetChildren())
	{
		updateTransformComponents(ECS::GetEntity(ID), matrix);
	}
}

/// <summary>
/// Updates all of the mesh components in this scene.
/// </summary>
void Scene::updateMeshComponents()
{
	for (MeshComponent& comp : ECS::GetMeshComponents())
	{
		int& transformIndex = ECS::GetEntity(comp.GetEntityID())->ComponentIndices.Transform;
		comp.SetModelMat(ECS::GetTransformComponent(transformIndex)->GetGlobalTransform());
	}
}
