#include "Window.h"

#include <iostream>

GLFWwindow* Window::g_WindowPtr;

unsigned int Window::g_WindowWidth = 1600;
unsigned int Window::g_WindowHeight = 1200;

bool Window::g_IsOpen = false;

void Window::InitWindow()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	g_WindowPtr = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "Game Engine", NULL, NULL);
	if (g_WindowPtr == NULL)
	{
		std::cout << "MAIN::ERROR::Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(g_WindowPtr);

	glfwSetFramebufferSizeCallback(g_WindowPtr, frameBufferSizeCallback);
	glfwSetInputMode(g_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "MAIN::ERROR::Failed to initialize OpenGL context!" << std::endl;
		return;
	}
	std::cout << "MAIN::INFO::Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;
	g_IsOpen = true;
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	g_WindowWidth = width;
	g_WindowHeight = height;
	glViewport(0, 0, g_WindowWidth, g_WindowHeight);
}

void Window::CloseWindow()
{
	glfwSetWindowShouldClose(g_WindowPtr, true);
	g_IsOpen = false;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(g_WindowPtr);
}

void Window::PollEvents()
{
	glfwPollEvents();
}