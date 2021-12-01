#include "Camera.h"

#include "../core/InputManager.h"
#include "../game/Settings.h"

glm::mat4 Camera_f::GetViewMatrix(const Camera& cam)
{
	return glm::lookAt(cam.LocalPosition, cam.LocalPosition + cam.Front, cam.Up);
}

void Camera_f::ApplyRotation(Camera& cam)
{

	// constrain pitch
	if (cam.Pitch > 89.0f)
		cam.Pitch = 89.0f;
	if (cam.Pitch < -89.0f)
		cam.Pitch = -89.0f;

	glm::vec3 newFront;
	newFront.x = cos(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
	newFront.y = sin(glm::radians(cam.Pitch));
	newFront.z = sin(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
	cam.Front = glm::normalize(newFront);

	cam.Right = glm::normalize(glm::cross(cam.Front, WorldUp));
	cam.Up = glm::normalize(glm::cross(cam.Right, cam.Front));
}

void Camera_f::ApplyTranslation(Camera& cam, const glm::vec3 translation)
{
	cam.LocalPosition += translation;
}
