#include "Input.h"

// defines the keybinds
void Input::Update(GLFWwindow* window)
{
	resetInput();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		ActionEscape = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		ActionMoveForward = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		ActionMoveBack = true;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ActionMoveLeft = true;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ActionMoveRight = true;
	}
}

void Input::resetInput()
{
	ActionEscape = false;
	ActionMoveForward = false;
	ActionMoveBack = false;
	ActionMoveLeft = false;
	ActionMoveRight = false;
}
