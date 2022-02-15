#include "Scene.h"

#include "../core/Log.h"
#include "../ECS/ECS.h"

/* -- PUBLIC -- */

void Scene::Update(const float& deltaTime)
{

}

std::shared_ptr<Entity> Scene::AddEntityToRoot(const std::string& name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	m_RootEntity->AddChild(entity);

	return entity;
}
