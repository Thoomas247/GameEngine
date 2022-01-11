#include "Window.h"

#include "Log.h"

GLFWwindow* Window::WindowPtr = nullptr;
bool Window::ShouldClose = false;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::InitWindow(const int& width, const int& height)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	WindowPtr = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
	if (WindowPtr == NULL)
	{
		LOG_ERROR("MAIN::Failed to create GLFW window!")
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(WindowPtr);

	glfwSetFramebufferSizeCallback(WindowPtr, frameBufferSizeCallback);
	glfwSetInputMode(WindowPtr, GLFW_CURSOR, /*GLFW_CURSOR_DISABLED*/ GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		LOG_ERROR("MAIN::Failed to initialize OpenGL context!")
		return;
	}
	LOG_INFO("MAIN::Loaded OpenGL " + std::string(GLAD_VERSION_MAJOR(version) + "." + GLAD_VERSION_MINOR(version)))
}

void Window::CloseWindow()
{
	glfwSetWindowShouldClose(WindowPtr, true);
	ShouldClose = true;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(WindowPtr);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::CleanUp()
{
	glfwTerminate();
}