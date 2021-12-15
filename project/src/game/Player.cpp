#include "Player.h"

#include <iostream>

#include "Settings.h"

// PUBLIC
Player::Player()
{
}

// PRIVATE
void Player::onUpdate(Data& data, const float& deltaTime)
{
	std::shared_ptr<GameObject> camera = GetChild("Camera");

	// Rotation
	camera->LocalRotation = glm::rotate(camera->LocalRotation, glm::radians(data.Input.MouseDeltaX * S_MouseSensitivity), glm::vec3(0.0f, -1.0f, 0.0f));
	camera->LocalRotation = glm::rotate(camera->LocalRotation, glm::radians(data.Input.MouseDeltaY * S_MouseSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

	// Movement
	glm::vec3 front = glm::mat3_cast(camera->LocalRotation) * glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::mat3_cast(camera->LocalRotation) * glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 inputVector = glm::vec3(0.0f);

	if (data.Input.ActionMoveForward)
	{
		inputVector.z += 1.0f;
	}
	if (data.Input.ActionMoveBack)
	{
		inputVector.z -= 1.0f;
	}
	if (data.Input.ActionMoveRight)
	{
		inputVector.x += 1.0f;
	}
	if (data.Input.ActionMoveLeft)
	{
		inputVector.x -= 1.0f;
	}

	if (inputVector != glm::vec3(0.0f))
	{
		inputVector = glm::normalize(inputVector);
		LocalPosition += front * inputVector.z * Speed * deltaTime;
		LocalPosition += right * inputVector.x * Speed * deltaTime;
		std::cout << LocalPosition.x << " : " << LocalPosition.y << " : " << LocalPosition.z << std::endl;
	}
}