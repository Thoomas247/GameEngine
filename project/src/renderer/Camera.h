#ifndef CAMERA
#define CAMERA

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../game/Settings.h"

struct Camera
{
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::vec3 GlobalPosition = glm::vec3(0.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

	float Yaw = 0.0f;
	float Pitch = 0.0f;

	float FOV = 75.0f;

	Camera() {}

	Camera(glm::vec3 localPosition, glm::vec3 facing) {
		LocalPosition = localPosition;
		GlobalPosition += LocalPosition;
		Front = facing;
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

namespace Camera_f
{
	glm::mat4 GetViewMatrix(const Camera& cam);
	void ApplyRotation(Camera& cam);
	void ApplyTranslation(Camera& cam, const glm::vec3 translation);
}

#endif // !CAMERA
