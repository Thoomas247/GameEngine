#include "Camera.h"

#include "../game/Settings.h"
#include "RenderData.h"

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

	m_ViewProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), S_ScreenWidth / S_ScreenHeight, m_NearPlane, m_FarPlane)
		* glm::lookAt(glm::vec3(m_GlobalTransform[3]), glm::vec3(m_GlobalTransform[3]) + front, up);
}

// PRIVATE
void Camera::onUpdate(const float& deltaTime)
{
	RenderData::CurrentCamera = this;
}