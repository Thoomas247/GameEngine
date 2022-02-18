#include "precompiled.h"
#include "TransformComponent.h"

#include "../Entity.h"

TransformComponent::TransformComponent(Entity* entity, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
: Component(entity)
{
	m_ParentID = entity->FindTransformParent();

	m_LocalTranslation = translation;
	m_LocalRotation = rotation;
	m_LocalScale = scale;

	m_LocalTransform = glm::translate(glm::mat4(1.0f), m_LocalTranslation) * glm::mat4_cast(m_LocalRotation) * glm::scale(glm::mat4(1.0f), m_LocalScale);
	m_GlobalTransform = m_LocalTransform;

	m_HasChanged = false;
}