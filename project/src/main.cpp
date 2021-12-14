#include <iostream>
#include <memory>

#include "external/GLIncludes.h"

#include "game/GameObject.h"
#include "game/Player.h"
#include "game/Settings.h"

#include "renderer/Camera.h"
#include "renderer/Shader.h"
#include "renderer/Mesh.h"

#include "core/Data.h"

Data data;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// make width and height global
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
	GameObject root;

	// debug: create shader
	Shader shader = Shader("C:/Users/TM1/source/repos/GameEngine/project/src/shaders/Base.vert", "C:/Users/TM1/source/repos/GameEngine/project/src/shaders/Base.frag");

	// debug: create triangle (in final code, creating a shared pointer will be handled by another function through object manager)
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(shader);
	root.AddChild("Mesh1", mesh);

	// debug: create player and camera
	std::shared_ptr<Player> player = std::make_shared<Player>();
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	root.AddChild("Player", player);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// main: program loop
	while (!glfwWindowShouldClose(window))
	{
		// main: update delta time
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << 1 / deltaTime << std::endl;	// fps count

		// main: update input globals
		data.Input.Update(window);

		// debug: detect if window should close
		if (data.Input.ActionEscape)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// main: update
		root.Update(data, deltaTime);

		// main: draw
		glClearColor(0.8f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->CalcViewProjectionMatrix(data);
		root.Draw(data);

		// main: swap buffers when done
		glfwSwapBuffers(window);

		// main: poll events
		glfwPollEvents();
	}
	return 0;
}