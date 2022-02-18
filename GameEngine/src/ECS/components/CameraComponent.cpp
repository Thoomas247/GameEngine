#include "CameraComponent.h"

CameraComponent::CameraComponent(Entity* entity, const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
	: Component(entity)
{
	m_FOV = fov;
	m_AspectRatio = aspectRatio;
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;

	m_ViewMatrix = glm::lookAt(glm::vec3(0.0f), m_WorldFront, m_WorldUp);
	m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);

	m_ProjectionChanged = false;
}