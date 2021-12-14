#include "Camera.h"

#include "../game/Settings.h"

// PUBLIC
Camera::Camera()
{
}

void Camera::CalcViewProjectionMatrix(Data& data)
{
	glm::vec3 front = GlobalTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 up = GlobalTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	data.ViewProjectionMatrix = glm::perspective<float>(glm::radians(FOV), S_ScreenWidth / S_ScreenHeight, NearPlane, FarPlane)
		* glm::lookAt(glm::vec3(GlobalTransform[3]), glm::vec3(GlobalTransform[3]) + front, up);
}

// PRIVATE
void Camera::onUpdate(Data& data, const float& deltaTime)
{
}