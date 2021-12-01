#include "Player.h"

#include "../core/InputManager.h"
#include "../core/Time.h"

void Player_f::Update(Player& player)
{
	// handle movement input:
	player.Yaw += Inputs::MouseDeltaX * MouseSensitivity;

	glm::vec3 newFront = glm::vec3(0.0f);
	newFront.x = cos(glm::radians(player.Yaw));
	newFront.z = sin(glm::radians(player.Yaw));
	player.Front = glm::normalize(newFront);

	glm::vec3 inputVector = glm::vec3(0.0f);

	if (Inputs::ActionMoveForward) {
		inputVector.z += -1.0f;
	}
	if (Inputs::ActionMoveBack) {
		inputVector.z += 1.0f;
	}
	if (Inputs::ActionMoveLeft) {
		inputVector.x += -1.0f;
	}
	if (Inputs::ActionMoveRight) {
		inputVector.x += 1.0f;
	}

	inputVector = glm::normalize(inputVector);

	player.GlobalPosition += player.Front * inputVector * player.Speed * Time::DeltaTime;

	// update camera:
	player.Camera.Pitch += Inputs::MouseDeltaY * MouseSensitivity;
	player.Camera.Yaw = player.Yaw;

}
