#pragma once

#include "glfw/glfw3.h"

class Input
{
private:
	static float s_MouseDeltaX;
	static float s_MouseDeltaY;

	static float s_MouseLastXPos;
	static float s_MouseLastYPos;

	static bool s_MouseRightButton;
	static bool s_MouseLeftButton;

	static bool s_ActionEscape;
	static bool s_ActionMoveForward;
	static bool s_ActionMoveBack;
	static bool s_ActionMoveLeft;
	static bool s_ActionMoveRight;

	// don't forget to update functions when adding keybinds!

public:
	static void Update();

	static float GetMouseDeltaX() { return s_MouseDeltaX; }
	static float GetMouseDeltaY() { return s_MouseDeltaY; }

	static bool GetMouseRightButton() { return s_MouseRightButton; }
	static bool GetMouseLeftButton() { return s_MouseLeftButton; }

	static bool GetActionEscape() { return s_ActionEscape; }
	static bool GetActionMoveForward() { return s_ActionMoveForward; }
	static bool GetActionMoveBack() { return s_ActionMoveBack; }
	static bool GetActionMoveLeft() { return s_ActionMoveLeft; }
	static bool GetActionMoveRight() { return s_ActionMoveRight; }

private:
	static void resetInput();
	static void setInput();
};