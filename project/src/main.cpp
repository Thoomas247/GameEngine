#include <iostream>
#include <memory>

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "importer/Importer.h"

#include "game/Player.h"
#include "game/Settings.h"

#include "renderer/Camera.h"
#include "renderer/Shader.h"
#include "renderer/Mesh.h"

#include "core/Root.h"
#include "core/GameObject.h"
#include "core/Data.h"
#include "core/AssetManager.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// TODO: make width and height global
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
		std::cout << "MAIN::ERROR::Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "MAIN::ERROR::Failed to initialize OpenGL context!" << std::endl;
		return -1;
	}
	std::cout << "MAIN::INFO::Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

	// opengl: enable settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	// opengl: configure settings
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// debug: import gltf file
	//Importer::ImportGLTF("Test", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");

	// main: init variables
	Root root;

	// debug: create shader
	Shader shader = Shader();

	// debug: create triangle (in final code, creating a shared pointer will be handled by another function through asset manager)
	std::shared_ptr<GameObject> bird = AssetManager::LoadModel("Test.GEM");
	root.AddChild("Bird", bird);

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
		Data::Inputs.Update(window);

		// debug: detect if window should close
		if (Data::Inputs.ActionEscape)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// main: update
		root.Update(deltaTime);

		// main: draw
		glClearColor(0.8f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->CalcViewProjectionMatrix();
		root.Draw();

		// main: swap buffers when done
		glfwSwapBuffers(window);

		// main: poll events
		glfwPollEvents();
	}
	return 0;
}