#pragma once

/*
* Implementation based on: 
*	https://vulkan-tutorial.com/Introduction
*	https://github.com/SaschaWillems/Vulkan/tree/master/base
*/

class Renderer
{
private:
	static glm::mat4 s_ViewMatrix;
	static glm::mat4 s_ProjectionMatrix;

public:
	static void Init();
	static void Draw();
	static void CleanUp();

	static void RequestResize();

	static void SetViewMatrix(const glm::mat4& viewMatrix) { s_ViewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const glm::mat4& projectionMatrix) { s_ProjectionMatrix = projectionMatrix; }
};