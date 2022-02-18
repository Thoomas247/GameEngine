#include "precompiled.h"
#include "Window.h"

GLFWwindow* Window::s_WindowPtr = nullptr;
bool Window::s_ShouldClose = false;
WindowSize Window::s_Size = WindowSize();


/* -- PUBLIC -- */

void Window::InitWindow(const int& width, const int& height)
{
	s_Size = WindowSize(width, height);

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	s_WindowPtr = glfwCreateWindow(s_Size.Width, s_Size.Height, "Game Engine", NULL, NULL);
	if (s_WindowPtr == nullptr)
	{
		glfwTerminate();
		LOG_ERROR("MAIN::Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(s_WindowPtr);

	glfwSetFramebufferSizeCallback(s_WindowPtr, frameBufferSizeCallback);
	glfwSetInputMode(s_WindowPtr, GLFW_CURSOR, /*GLFW_CURSOR_DISABLED*/ GLFW_CURSOR_NORMAL);
	//glfwSwapInterval(1);	// vsync

	// glad: load all OpenGL function pointers
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		LOG_ERROR("MAIN::Failed to initialize OpenGL context!");
		return;
	}

	std::string ver = std::to_string(GLAD_VERSION_MAJOR(version)) + "." + std::to_string(GLAD_VERSION_MINOR(version));
	LOG_INFO("MAIN::Loaded OpenGL " + ver);
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


/* -- PRIVATE -- */

void Window::frameBufferSizeCallback(GLFWwindow*, int width, int height)
{
	s_Size = WindowSize(width, height);
	glViewport(0, 0, s_Size.Width, s_Size.Height);
}