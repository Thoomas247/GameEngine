#include "Camera.h"

#include "../game/Settings.h"

// PUBLIC
Camera::Camera()
{

}

void Camera::Update(Data& data, const float deltaTime, const glm::mat4& parentTransform)
{
	GlobalTransform = parentTransform * glm::mat4_cast(LocalRotation) * glm::translate(glm::mat4(1.0f), LocalPosition);

	glm::vec3 front = GlobalTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 up = GlobalTransform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	ViewProjectionMatrix = glm::perspective<float>(glm::radians(FOV), S_ScreenWidth / S_ScreenHeight, NearPlane, FarPlane)
		* glm::lookAt(glm::vec3(GlobalTransform[3]), glm::vec3(GlobalTransform[3]) + front, up);

	data.ViewProjectionMatrix = ViewProjectionMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return ViewProjectionMatrix;
}


