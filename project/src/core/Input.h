#ifndef INPUT
#define INPUT

#include "glfw/glfw3.h"

class Input
{
public:
	float MouseDeltaX = 0.0f;
	float MouseDeltaY = 0.0f;

	bool ActionEscape = false;
	bool ActionMoveForward = false;
	bool ActionMoveBack = false;
	bool ActionMoveLeft = false;
	bool ActionMoveRight = false;
	// don't forget to update functions when adding!

private:
	float MouseLastXPos = 0.0f;
	float MouseLastYPos = 0.0f;

public:
	void Update(GLFWwindow* window);

private:
	void resetInput();
};

#endif // !INPUT
