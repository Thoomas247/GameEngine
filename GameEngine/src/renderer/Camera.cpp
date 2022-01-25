#include "Camera.h"

#include "Renderer.h"

// PUBLIC
Camera::Camera()
{
}

void Camera::CalcViewProjectionMatrix()
{
	if (m_GlobalTransform == m_LastGlobalTransform)
	{
		return;
	}
	m_LastGlobalTransform = m_GlobalTransform;

	glm::vec3 front = m_GlobalTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 up = m_GlobalTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	ViewMatrix = glm::lookAt(glm::vec3(m_GlobalTransform[3]), glm::vec3(m_GlobalTransform[3]) + front, up);
	ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), AspectRatio, m_NearPlane, m_FarPlane);	// TODO: make callback instead of calc every frame
}

// PRIVATE
void Camera::onUpdate(const float& deltaTime)
{
	
}