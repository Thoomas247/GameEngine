#include "main.h"

#include "renderer/Camera.h"
#include "game/Player.h"
#include "core/InputManager.h"
#include "core/Time.h"

int main() 
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// opengl: enable settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	// opengl: configure settings
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// main: init variables
	Player player;

	// main: program loop
	while (!glfwWindowShouldClose(window)) 
	{
		float currentFrame = float(glfwGetTime());
		Time::DeltaTime = currentFrame - Time::LastFrame;
		Time::LastFrame = currentFrame;
		std::cout << 1 / Time::DeltaTime << std::endl;	// fps count

		InputManager_f::UpdateInput(window);	// do first

		if (Inputs::ActionEscape)	// temp for debugging
		{
			glfwSetWindowShouldClose(window, true);
		}

		glfwPollEvents();	// do last
	}
	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// make width and height global
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Inputs::MouseDeltaX = xpos - Inputs::MouseLastXPos;
	Inputs::MouseDeltaY = Inputs::MouseLastYPos - ypos;	// y coords are reversed

	Inputs::MouseLastXPos = xpos;
	Inputs::MouseLastYPos = ypos;
}
