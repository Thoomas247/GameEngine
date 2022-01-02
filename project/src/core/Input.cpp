#include "Input.h"

#include "Window.h"

float Input::g_MouseDeltaX = 0.0f;
float Input::g_MouseDeltaY = 0.0f;
float Input::g_MouseLastXPos = 0.0f;
float Input::g_MouseLastYPos = 0.0f;

bool Input::g_ActionEscape = false;
bool Input::g_ActionMoveForward = false;
bool Input::g_ActionMoveBack = false;
bool Input::g_ActionMoveLeft = false;
bool Input::g_ActionMoveRight = false;

// defines the keybinds
void Input::Update()
{
	double xpos, ypos;
	glfwGetCursorPos(Window::g_WindowPtr, &xpos, &ypos);
	g_MouseDeltaX = float(xpos) - g_MouseLastXPos;
	g_MouseDeltaY = g_MouseLastYPos - float(ypos);
	g_MouseLastXPos = xpos;
	g_MouseLastYPos = ypos;

	resetInput();
	setInput();
}

void Input::resetInput()
{
	g_ActionEscape = false;
	g_ActionMoveForward = false;
	g_ActionMoveBack = false;
	g_ActionMoveLeft = false;
	g_ActionMoveRight = false;
}

void Input::setInput()
{
	if (glfwGetKey(Window::g_WindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		g_ActionEscape = true;
	}

	if (glfwGetKey(Window::g_WindowPtr, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_ActionMoveForward = true;
	}

	if (glfwGetKey(Window::g_WindowPtr, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_ActionMoveBack = true;
	}

	if (glfwGetKey(Window::g_WindowPtr, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_ActionMoveLeft = true;
	}

	if (glfwGetKey(Window::g_WindowPtr, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_ActionMoveRight = true;
	}
}