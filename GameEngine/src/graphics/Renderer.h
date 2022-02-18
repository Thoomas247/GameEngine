#pragma once

#include "glm/glm.hpp"

class Renderer
{
private:
	static glm::mat4 s_ViewMatrix;
	static glm::mat4 s_ProjectionMatrix;

public:
	static void Init();
	static void Draw();

	static void SetViewMatrix(const glm::mat4& viewMatrix) { s_ViewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const glm::mat4& projectionMatrix) { s_ProjectionMatrix = projectionMatrix; }
};