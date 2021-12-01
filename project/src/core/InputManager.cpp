#include "InputManager.h"

float Inputs::MouseLastXPos = 0.0f;
float Inputs::MouseLastYPos = 0.0f;
float Inputs::MouseDeltaX = 0.0f;
float Inputs::MouseDeltaY = 0.0f;

bool Inputs::ActionEscape = false;
bool Inputs::ActionMoveForward = false;
bool Inputs::ActionMoveBack = false;
bool Inputs::ActionMoveLeft = false;
bool Inputs::ActionMoveRight = false;

// defines the keybinds
void InputManager_f::UpdateInput(GLFWwindow* window)
{
	InputManager_f::ResetInput();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		Inputs::ActionEscape = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Inputs::ActionMoveForward = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Inputs::ActionMoveBack = true;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Inputs::ActionMoveLeft = true;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Inputs::ActionMoveRight = true;
	}
}

void InputManager_f::ResetInput()
{
	Inputs::ActionEscape = false;
	Inputs::ActionMoveForward = false;
	Inputs::ActionMoveBack = false;
	Inputs::ActionMoveLeft = false;
	Inputs::ActionMoveRight = false;
}
