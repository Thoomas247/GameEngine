#include "Player.h"

// PUBLIC
void Player::Update(Data& data, const float& deltaTime, const glm::mat4& parentTransform)
{
	// handle movement input:
	glm::vec3 up = glm::mat3_cast(LocalRotation) * glm::vec3(0.0f, 1.0f, 0.0f);
	float yawDelta = data.Input.MouseDeltaX * S_MouseSensitivity;
	LocalRotation = glm::rotate(LocalRotation, yawDelta, up);

	glm::vec3 inputVector = glm::vec3(0.0f);

	if (data.Input.ActionMoveForward) {
		inputVector.z += -1.0f;
	}
	if (data.Input.ActionMoveBack) {
		inputVector.z += 1.0f;
	}
	if (data.Input.ActionMoveLeft) {
		inputVector.x += -1.0f;
	}
	if (data.Input.ActionMoveRight) {
		inputVector.x += 1.0f;
	}

	if (inputVector != glm::vec3(0.0f)) {
		inputVector = glm::normalize(inputVector);
		LocalPosition += glm::mat3_cast(LocalRotation) * (inputVector * Speed * deltaTime);
	}

	// update transform:
	GlobalTransform = parentTransform * glm::mat4_cast(LocalRotation) * glm::translate(glm::mat4(1.0f), LocalPosition);

	// update children:
	Camera.Update(data, deltaTime, GlobalTransform);
	for (auto& [key, mesh] : MeshChildren)
	{
		mesh.Update(data, deltaTime, GlobalTransform);
	}

}

void Player::AddChildMesh(const std::string& name, Mesh& mesh)
{
}
