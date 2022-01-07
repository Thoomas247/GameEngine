#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "glfw/glfw3.h"

namespace Input
{
	extern float g_MouseDeltaX;
	extern float g_MouseDeltaY;

	extern float g_MouseLastXPos;
	extern float g_MouseLastYPos;

	extern bool g_ActionEscape;
	extern bool g_ActionMoveForward;
	extern bool g_ActionMoveBack;
	extern bool g_ActionMoveLeft;
	extern bool g_ActionMoveRight;
	// don't forget to update functions when adding!

	void Update();

	void resetInput();
	void setInput();
}

#endif // !INPUT_MANAGER
