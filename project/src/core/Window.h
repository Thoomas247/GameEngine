#ifndef GLFW_WINDOW
#define GLFW_WINDOW

#include "glad/gl.h"
#include "glfw/glfw3.h"

namespace Window
{
	extern GLFWwindow* g_WindowPtr;

	extern unsigned int g_WindowWidth;
	extern unsigned int g_WindowHeight;

	extern bool g_IsOpen;

	void InitWindow();
	void CloseWindow();
	void SwapBuffers();
	void PollEvents();

	void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
}

#endif // !GLFW_WINDOW