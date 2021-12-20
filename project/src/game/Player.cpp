#include "Player.h"

#include <iostream>

#include "Settings.h"
#include "../core/Data.h"

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
	m_Camera->m_LocalRotation = glm::rotate(m_Camera->m_LocalRotation, glm::radians(Data::Inputs.MouseDeltaX * S_MouseSensitivity), glm::vec3(0.0f, -1.0f, 0.0f));
	m_Camera->m_LocalRotation = glm::rotate(m_Camera->m_LocalRotation, glm::radians(Data::Inputs.MouseDeltaY * S_MouseSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

	// Movement
	glm::vec3 front = glm::mat3_cast(m_Camera->m_LocalRotation) * glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::mat3_cast(m_Camera->m_LocalRotation) * glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 inputVector = glm::vec3(0.0f);

	if (Data::Inputs.ActionMoveForward)
	{
		inputVector.z += 1.0f;
	}
	if (Data::Inputs.ActionMoveBack)
	{
		inputVector.z -= 1.0f;
	}
	if (Data::Inputs.ActionMoveRight)
	{
		inputVector.x += 1.0f;
	}
	if (Data::Inputs.ActionMoveLeft)
	{
		inputVector.x -= 1.0f;
	}

	if (inputVector != glm::vec3(0.0f))
	{
		inputVector = glm::normalize(inputVector);
		m_LocalPosition += front * inputVector.z * m_Speed * deltaTime;
		m_LocalPosition += right * inputVector.x * m_Speed * deltaTime;
	}
}