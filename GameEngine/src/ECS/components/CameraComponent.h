#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Component.h"

class CameraComponent : public Component
{
private:
	glm::vec3 m_WorldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	float m_FOV;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	bool m_ProjectionChanged;

public:
	CameraComponent(Entity* entity, const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
		: Component(entity)
	{
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;

		m_ViewMatrix = glm::lookAt(glm::vec3(0.0f), m_WorldFront, m_WorldUp);
		m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);

		m_ProjectionChanged = false;
	}

	/*
	void UpdateViewAndProjection()
	{
		if (m_ProjectionChanged)
		{
			m_ProjectionMatrix = glm::perspective<float>(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		}

		glm::mat3 rotationMat = glm::mat3(m_Entity->GetTransformComponent().GetGlobalTransform())

		glm::vec3 front = rotationMat * m_WorldFront;
		glm::vec3 up = rotationMat * m_WorldUp;

		m_ViewMatrix = glm::lookAt(m_Translation, m_Translation + front, up);
	}
	*/

	void SetFOV(const float& fov) { m_FOV = fov; m_ProjectionChanged = true; }
	void SetNearPlane(const float& nearPlane) { m_NearPlane = nearPlane; m_ProjectionChanged = true; }
	void SetFarPlane(const float& farPlane) { m_FarPlane = farPlane; m_ProjectionChanged = true; }

	const glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
	const glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }
};