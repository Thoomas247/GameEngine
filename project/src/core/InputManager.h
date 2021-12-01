#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "glfw/glfw3.h"

namespace Inputs 
{
	extern float MouseLastXPos;
	extern float MouseLastYPos;
	extern float MouseDeltaX;
	extern float MouseDeltaY;

	extern bool ActionEscape;
	extern bool ActionMoveForward;
	extern bool ActionMoveBack;
	extern bool ActionMoveLeft;
	extern bool ActionMoveRight;
	// don't forget to update ResetInput()!
}

namespace InputManager_f 
{
	void UpdateInput(GLFWwindow* window);
	void ResetInput();
}

#endif // !INPUT_MANAGER
