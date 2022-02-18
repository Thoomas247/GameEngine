#include "precompiled.h"
#include "CameraComponent.h"

/* -- PUBLIC -- */

CameraComponent::CameraComponent(Entity* entity, const float& fov, const float& nearPlane, const float& farPlane)
	: Component(entity)
{
	m_Active = false;

	m_FOV = fov;
	m_AspectRatio = Window::GetSize().AspectRatio();
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;

	m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

const glm::mat4 CameraComponent::GetProjectionMatrix()
{
	if (m_AspectRatio != Window::GetSize().AspectRatio())
	{
		m_AspectRatio = Window::GetSize().AspectRatio();
		recalculateProjection();
	}

	return m_ProjectionMatrix;
}


/* -- PRIVATE -- */

void CameraComponent::recalculateProjection()
{
	m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}