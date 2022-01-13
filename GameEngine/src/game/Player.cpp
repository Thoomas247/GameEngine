#include "Player.h"

#include <iostream>

#include "Settings.h"
#include "../core/Input.h"

// PUBLIC
Player::Player()
{
}

// PRIVATE
void Player::onSetUp()
{
	m_Camera = GetChild("Camera");
}

void Player::onUpdate(const float& deltaTime)
{
	// Rotation
	m_Camera->LocalRotation = glm::rotate(m_Camera->LocalRotation, glm::radians(Input::MouseDeltaX * S_MouseSensitivity), glm::vec3(0.0f, -1.0f, 0.0f));
	m_Camera->LocalRotation = glm::rotate(m_Camera->LocalRotation, glm::radians(Input::MouseDeltaY * S_MouseSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

	// Movement
	glm::vec3 front = glm::mat3_cast(m_Camera->LocalRotation) * glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::mat3_cast(m_Camera->LocalRotation) * glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 inputVector = glm::vec3(0.0f);

	if (Input::ActionMoveForward)
	{
		inputVector.z += 1.0f;
	}
	if (Input::ActionMoveBack)
	{
		inputVector.z -= 1.0f;
	}
	if (Input::ActionMoveRight)
	{
		inputVector.x += 1.0f;
	}
	if (Input::ActionMoveLeft)
	{
		inputVector.x -= 1.0f;
	}

	if (inputVector != glm::vec3(0.0f))
	{
		inputVector = glm::normalize(inputVector);
		LocalPosition += front * inputVector.z * m_Speed * deltaTime;
		LocalPosition += right * inputVector.x * m_Speed * deltaTime;
	}
}