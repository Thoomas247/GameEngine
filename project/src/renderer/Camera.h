#ifndef CAMERA
#define CAMERA

/*
	Camera class
	A GameObject with an extra GetViewProjectionMatrix() method
	which the renderer calls on the current camera before
	drawing all meshes
*/

#include "../core/GameObject.h"

class Camera : public GameObject {
public:
	glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

private:
	glm::mat4 m_LastGlobalTransform = glm::mat4(1.0f);

	float m_FarPlane = 10000.0f;
	float m_NearPlane = 0.1f;
	float m_FOV = 75.0f;

public:
	Camera();
	void CalcViewProjectionMatrix();

private:
	void onUpdate(const float& deltaTime) override;
};

#endif // !CAMERA
