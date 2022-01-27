#include "Window.h"

#include "Log.h"

GLFWwindow* Window::s_WindowPtr = nullptr;
bool Window::s_ShouldClose = false;

// PUBLIC
void Window::InitWindow(const int& width, const int& height)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	s_WindowPtr = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
	if (s_WindowPtr == NULL)
	{
		LOG_ERROR("MAIN::Failed to create GLFW window!")
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(s_WindowPtr);

	glfwSetFramebufferSizeCallback(s_WindowPtr, frameBufferSizeCallback);
	glfwSetInputMode(s_WindowPtr, GLFW_CURSOR, /*GLFW_CURSOR_DISABLED*/ GLFW_CURSOR_NORMAL);
	glfwSwapInterval(1);	// vsync

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
	glfwSetWindowShouldClose(s_WindowPtr, true);
	s_ShouldClose = true;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(s_WindowPtr);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::CleanUp()
{
	glfwTerminate();
}

void Window::LockCursor()
{
	glfwSetInputMode(s_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::UnlockCursor()
{
	glfwSetInputMode(s_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// PRIVATE
void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}