#include "Scene.h"

#include <vector>

#include "glm/glm.hpp"

#include "../core/Log.h"
#include "../ECS/ECS.h"
#include "../ECS/components/TransformComponent.h"

/* -- PUBLIC -- */

void Scene::Update(const float& deltaTime)
{
	// transforms update:
	std::vector<bool> dirty = std::vector<bool>(ECS::GetNumComponents<TransformComponent>(), true);
	auto& transformComponents = ECS::GetComponents<TransformComponent>();

	bool done = false;
	while (!done)
	{
		done = true;

		for (int i = 0; i < ECS::GetNumComponents<TransformComponent>(); i++)
		{
			TransformComponent& transform = transformComponents[i];

			int parentIndex = transform.GetParentIndex();

			if (parentIndex == -1)	// update if it has no parent
			{
				transform.UpdateTransforms(glm::mat4(1.0f));
				dirty[i] = false;
				continue;
			}

			if (dirty[parentIndex])	// skip if parent hasn't updated yet
			{
				done = false;
				continue;
			}

			// otherwise update normally
			TransformComponent& parent = ECS::GetComponent<TransformComponent>(parentIndex);
			transform.UpdateTransforms(parent.GetGlobalTransform());
			dirty[i] = false;
		}
	}
}

std::shared_ptr<Entity> Scene::AddEntityToRoot(const std::string& name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name, m_RootEntity.get());
	m_RootEntity->AddChild(entity);

	return entity;
}
