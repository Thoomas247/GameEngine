#pragma once

#include "glfw/glfw3.h"

namespace Input
{
	extern float MouseDeltaX;
	extern float MouseDeltaY;

	extern float MouseLastXPos;
	extern float MouseLastYPos;


	extern bool MouseRightButton;
	extern bool MouseLeftButton;

	extern bool ActionEscape;
	extern bool ActionMoveForward;
	extern bool ActionMoveBack;
	extern bool ActionMoveLeft;
	extern bool ActionMoveRight;
	// don't forget to update functions when adding!

	void Update();

	void resetInput();
	void setInput();
}