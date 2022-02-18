#pragma once

#include "glad/gl.h"
#include "glfw/glfw3.h"

struct WindowSize
{
	int Width;
	int Height;

	float AspectRatio() { return (float)Width / Height; }

	WindowSize(const int& width = 1600, const int& height = 900)
	{
		Width = width;
		Height = height;
	}
};

class Window
{
private:
	static GLFWwindow* s_WindowPtr;
	static bool s_ShouldClose;
	static WindowSize s_Size;

public:
	static void InitWindow(const int& width, const int& height);
	static void CloseWindow();
	static void SwapBuffers();
	static void PollEvents();
	static void CleanUp();

	static WindowSize GetSize() { return s_Size; }

	static void LockCursor();
	static void UnlockCursor();

	static GLFWwindow* GetWindowPtr() { return s_WindowPtr; }
	static void SetWindowPtr(GLFWwindow* newPtr) { s_WindowPtr = newPtr; glfwMakeContextCurrent(s_WindowPtr); }

	static bool ShouldClose() { return s_ShouldClose; }

private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};