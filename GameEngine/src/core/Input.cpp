#include "Input.h"

#include "Window.h"

float Input::s_MouseDeltaX = 0.0f;
float Input::s_MouseDeltaY = 0.0f;
float Input::s_MouseLastXPos = 0.0f;
float Input::s_MouseLastYPos = 0.0f;

bool Input::s_MouseLeftButton = false;
bool Input::s_MouseRightButton = false;

bool Input::s_ActionEscape = false;
bool Input::s_ActionMoveForward = false;
bool Input::s_ActionMoveBack = false;
bool Input::s_ActionMoveLeft = false;
bool Input::s_ActionMoveRight = false;

// PUBLIC
void Input::Update()
{
	double xpos, ypos;
	glfwGetCursorPos(Window::GetWindowPtr(), &xpos, &ypos);
	s_MouseDeltaX = (float)xpos - s_MouseLastXPos;
	s_MouseDeltaY = s_MouseLastYPos - (float)ypos;
	s_MouseLastXPos = (float)xpos;
	s_MouseLastYPos = (float)ypos;

	resetInput();
	setInput();
}

// PRIVATE
void Input::resetInput()
{
	s_MouseLeftButton = false;
	s_MouseRightButton = false;

	s_ActionEscape = false;
	s_ActionMoveForward = false;
	s_ActionMoveBack = false;
	s_ActionMoveLeft = false;
	s_ActionMoveRight = false;
}

void Input::setInput()
{
	if (glfwGetMouseButton(Window::GetWindowPtr(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		s_MouseLeftButton = true;
	}

	if (glfwGetMouseButton(Window::GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		s_MouseRightButton = true;
	}

	if (glfwGetKey(Window::GetWindowPtr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		s_ActionEscape = true;
	}

	if (glfwGetKey(Window::GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
	{
		s_ActionMoveForward = true;
	}

	if (glfwGetKey(Window::GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
	{
		s_ActionMoveBack = true;
	}

	if (glfwGetKey(Window::GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
	{
		s_ActionMoveLeft = true;
	}

	if (glfwGetKey(Window::GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
	{
		s_ActionMoveRight = true;
	}
}