#include <iostream>

#include "external/GLIncludes.h"

#include "renderer/Camera.h"
#include "renderer/Shader.h"
#include "renderer/Mesh.h"
#include "game/WorldRoot.h"
#include "game/Player.h"
#include "game/Settings.h"
#include "core/Data.h"

Data data;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// make width and height global
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	data.Input.MouseDeltaX = float(xpos) - data.Input.MouseLastXPos;
	data.Input.MouseDeltaY = data.Input.MouseLastYPos - float(ypos);	// y coords are reversed

	data.Input.MouseLastXPos = float(xpos);
	data.Input.MouseLastYPos = float(ypos);
}

int main() 
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: window creation
	GLFWwindow* window = glfwCreateWindow(S_ScreenWidth, S_ScreenHeight, "Game Engine", NULL, NULL);
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
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// main: init variables
	WorldRoot world;

	// debug: create shader
	Shader shader = Shader("C:/Users/TM1/source/repos/GameEngine/project/src/shaders/Base.vert", "C:/Users/TM1/source/repos/GameEngine/project/src/shaders/Base.frag");

	// debug: create triangle
	Mesh mesh = Mesh(shader);
	mesh.MakeTri();
	world.AddChildMesh("Mesh1", mesh);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// main: program loop
	while (!glfwWindowShouldClose(window)) 
	{
		// main: update delta time
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::cout << 1 / deltaTime << std::endl;	// fps count

		// main: update input globals
		data.Input.Update(window);

		// debug: detect if window should close
		if (data.Input.ActionEscape)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// main: draw
		glClearColor(0.8f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// main: update
		world.Update(data, deltaTime);

		// main: swap buffers when done
		glfwSwapBuffers(window);

		// main: poll events
		glfwPollEvents();
	}
	return 0;
}
