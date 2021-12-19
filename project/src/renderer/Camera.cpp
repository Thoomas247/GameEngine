#include "Camera.h"

#include "../game/Settings.h"

// PUBLIC
Camera::Camera()
{
}

void Camera::CalcViewProjectionMatrix()
{
	glm::vec3 front = m_GlobalTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 up = m_GlobalTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	Data::ViewProjectionMatrix = glm::perspective<float>(glm::radians(FOV), S_ScreenWidth / S_ScreenHeight, NearPlane, FarPlane)
		* glm::lookAt(glm::vec3(m_GlobalTransform[3]), glm::vec3(m_GlobalTransform[3]) + front, up);
}

// PRIVATE
void Camera::onUpdate(const float& deltaTime)
{
}