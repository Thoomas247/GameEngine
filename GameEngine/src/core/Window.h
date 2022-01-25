#pragma once

#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace Window
{
	extern GLFWwindow* WindowPtr;
	extern bool ShouldClose;

	void InitWindow(const int& width, const int& height);
	void CloseWindow();
	void SwapBuffers();
	void PollEvents();
	void CleanUp();

	void LockCursor();
	void UnlockCursor();
}