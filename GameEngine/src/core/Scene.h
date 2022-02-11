#pragma once

#include "../component system/ECS.h"

class Scene
{
private:
	uint64_t m_RootID;

public:
	Scene()
	{
		m_RootID = ECS::CreateEntity("Root");
	}

	void Update(const float& deltaTime);

private:
	void updateTransformComponents(Entity* entity, const glm::mat4& parentTransform);
	void updateMeshComponents();
};