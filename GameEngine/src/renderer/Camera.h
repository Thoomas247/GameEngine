#pragma once

/*
	Camera class
	A GameObject with an extra GetViewProjectionMatrix() method
	which the renderer calls on the current camera before
	drawing all meshes
*/

#include "../core/GameObject.h"

class Camera : public GameObject 
{
private:
	glm::mat4 m_LastGlobalTransform = glm::mat4(1.0f);

	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

	float m_FarPlane = 10000.0f;
	float m_NearPlane = 0.1f;
	float m_FOV = 75.0f;
	float m_AspectRatio = 16 / 9;

public:
	Camera();
	void CalcViewProjectionMatrix();

	glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

	glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	void SetAspectRatio(const float& aspectRatio) { m_AspectRatio = aspectRatio; }


private:
	void onUpdate(const float& deltaTime) override;
};
