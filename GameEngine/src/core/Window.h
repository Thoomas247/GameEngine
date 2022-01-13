#pragma once

#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace Window
{
	extern GLFWwindow* WindowPtr;
	extern bool ShouldClose;

	extern int GameWindowWidth;
	extern int GameWindowHeight;

	void InitWindow(const int& width, const int& height);
	void CloseWindow();
	void SwapBuffers();
	void PollEvents();
	void CleanUp();
}