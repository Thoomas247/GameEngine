#ifndef CAMERA
#define CAMERA

/*
	Camera class
	A GameObject with an extra GetViewProjectionMatrix() method
	which the renderer calls on the current camera before
	drawing all meshes
*/

#include "../game/GameObject.h"

class Camera : public GameObject {
private:
	glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

	float FarPlane = 1000.0f;
	float NearPlane = 0.1f;
	float FOV = 75.0f;

public:
	Camera();
	void CalcViewProjectionMatrix(Data& data);

private:
	void onUpdate(Data& data, const float& deltaTime) override;
};

#endif // !CAMERA
