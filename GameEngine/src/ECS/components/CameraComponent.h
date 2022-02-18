#pragma once
#include "Component.h"

class CameraComponent : public Component
{
private:
	glm::vec3 m_WorldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 m_ProjectionMatrix;

	bool m_Active;

	float m_FOV;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

public:
	CameraComponent(Entity* entity, const float& fov = 90.0f, const float& nearPlane = 0.1f, const float& farPlane = 1000.0f);

	bool IsActive() { return m_Active; }
	void SetActive(bool active) { m_Active = active; }

	void SetFOV(const float& fov) { m_FOV = fov; recalculateProjection(); }
	void SetNearPlane(const float& nearPlane) { m_NearPlane = nearPlane; recalculateProjection(); }
	void SetFarPlane(const float& farPlane) { m_FarPlane = farPlane; recalculateProjection(); }

	const glm::mat4 GetProjectionMatrix();

private:
	void recalculateProjection();
};