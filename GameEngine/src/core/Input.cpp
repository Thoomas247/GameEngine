#include "Input.h"

#include "Window.h"

float Input::MouseDeltaX = 0.0f;
float Input::MouseDeltaY = 0.0f;
float Input::MouseLastXPos = 0.0f;
float Input::MouseLastYPos = 0.0f;

bool Input::ActionEscape = false;
bool Input::ActionMoveForward = false;
bool Input::ActionMoveBack = false;
bool Input::ActionMoveLeft = false;
bool Input::ActionMoveRight = false;

// defines the keybinds
void Input::Update()
{
	double xpos, ypos;
	glfwGetCursorPos(Window::WindowPtr, &xpos, &ypos);
	MouseDeltaX = (float)xpos - MouseLastXPos;
	MouseDeltaY = MouseLastYPos - (float)ypos;
	MouseLastXPos = (float)xpos;
	MouseLastYPos = (float)ypos;

	resetInput();
	setInput();
}

void Input::resetInput()
{
	ActionEscape = false;
	ActionMoveForward = false;
	ActionMoveBack = false;
	ActionMoveLeft = false;
	ActionMoveRight = false;
}

void Input::setInput()
{
	if (glfwGetKey(Window::WindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		ActionEscape = true;
	}

	if (glfwGetKey(Window::WindowPtr, GLFW_KEY_W) == GLFW_PRESS)
	{
		ActionMoveForward = true;
	}

	if (glfwGetKey(Window::WindowPtr, GLFW_KEY_S) == GLFW_PRESS)
	{
		ActionMoveBack = true;
	}

	if (glfwGetKey(Window::WindowPtr, GLFW_KEY_A) == GLFW_PRESS)
	{
		ActionMoveLeft = true;
	}

	if (glfwGetKey(Window::WindowPtr, GLFW_KEY_D) == GLFW_PRESS)
	{
		ActionMoveRight = true;
	}
}