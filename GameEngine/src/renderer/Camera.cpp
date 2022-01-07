#include "Camera.h"

#include "../core/Window.h"
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

	//m_ViewProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), S_ScreenWidth / S_ScreenHeight, m_NearPlane, m_FarPlane)
	//	* glm::lookAt(glm::vec3(m_GlobalTransform[3]), glm::vec3(m_GlobalTransform[3]) + front, up);

	m_ViewMatrix = glm::lookAt(glm::vec3(m_GlobalTransform[3]), glm::vec3(m_GlobalTransform[3]) + front, up);
	m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), Window::g_WindowWidth / Window::g_WindowHeight, m_NearPlane, m_FarPlane);	// TODO: make callback instead of calc every frame
}

// PRIVATE
void Camera::onUpdate(const float& deltaTime)
{
	Renderer::g_CurrentCamera = this;
}