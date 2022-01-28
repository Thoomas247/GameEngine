#pragma once

#include "glad/gl.h"
#include "glfw/glfw3.h"

class Window
{
private:
	static GLFWwindow* s_WindowPtr;
	static bool s_ShouldClose;

public:
	static void InitWindow(const int& width, const int& height);
	static void CloseWindow();
	static void SwapBuffers();
	static void PollEvents();
	static void CleanUp();

	static void LockCursor();
	static void UnlockCursor();

	static GLFWwindow* GetWindowPtr() { return s_WindowPtr; }
	static void SetWindowPtr(GLFWwindow* newPtr) { s_WindowPtr = newPtr; glfwMakeContextCurrent(s_WindowPtr); }

	static bool ShouldClose() { return s_ShouldClose; }

private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};