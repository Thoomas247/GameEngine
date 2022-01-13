#include "Camera.h"

#include "Renderer.h"

// PUBLIC
Camera::Camera()
{
}

void Camera::CalcViewProjectionMatrix()
{
	if (GlobalTransform == m_LastGlobalTransform)
	{
		return;
	}
	m_LastGlobalTransform = GlobalTransform;

	glm::vec3 front = GlobalTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 up = GlobalTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	ViewMatrix = glm::lookAt(glm::vec3(GlobalTransform[3]), glm::vec3(GlobalTransform[3]) + front, up);
	ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), AspectRatio, m_NearPlane, m_FarPlane);	// TODO: make callback instead of calc every frame
}

// PRIVATE
void Camera::onUpdate(const float& deltaTime)
{
	Renderer::CurrentCamera = this;	// TODO: Remove this and set camera in GUI
}