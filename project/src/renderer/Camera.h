#ifndef CAMERA
#define CAMERA

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../core/Data.h"
#include "../game/Settings.h"

class Camera {

private:
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::quat LocalRotation = glm::quat();

	glm::mat4 GlobalTransform = glm::mat4(1.0f);	// position and rotation in world space

	glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

	float Yaw = 0.0f;
	float Pitch = 0.0f;

	float FarPlane = 1000.0f;
	float NearPlane = 0.1f;
	float FOV = 75.0f;

public:
	Camera();
	void Update(Data& data, const float deltaTime, const glm::mat4& parentTransform = glm::mat4(1.0f));
	glm::mat4 GetViewProjectionMatrix();
};

#endif // !CAMERA
