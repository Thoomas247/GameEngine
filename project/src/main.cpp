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
#include "renderer/Renderer.h"
#include "renderer/Animator.h"

#include "core/World.h"
#include "core/GameObject.h"
#include "core/Data.h"
#include "core/ModelLoader.h"

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


	// main: reserve space in renderer/animator lists
	Renderer::Reserve();
	Animator::Reserve();

	// debug: import gltf file
	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Viking", "F:/Users/TM1/Downloads/viking_room/scene.gltf");
	//Importer::ImportGLTF("Bat", "F:/Users/TM1/Downloads/batmobile_low_poly/scene.gltf");
	//Importer::ImportGLTF("Snowfox", "C:/Users/Thomas/Downloads/Snowfox/scene.gltf");

	// debug: load meshes
	std::shared_ptr<GameObject> viking = ModelLoader::LoadModel("Viking.GEM");
	World::AddGameObject("Viking", viking);

	std::shared_ptr<GameObject> bird = ModelLoader::LoadModel("Bird.GEM");
	bird->m_LocalScale = glm::vec3(0.1f, 0.1f, 0.1f);
	bird->m_LocalPosition = glm::vec3(0.0f, 40.0f, 0.0f);
	World::AddGameObject("Bird", bird);

	std::shared_ptr<GameObject> snowfox = ModelLoader::LoadModel("Snowfox.GEM");
	snowfox->m_LocalPosition = glm::vec3(40.0f, 0.0f, 0.0f);
	World::AddGameObject("Snowfox", snowfox);

	//std::shared_ptr<GameObject> bat = ModelLoader::LoadModel("Bat.GEM");
	//World::AddGameObject("Bat", bat);

	// debug: create player and camera
	std::shared_ptr<Player> player = std::make_shared<Player>();
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	World::AddGameObject("Player", player);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// main: setup
	World::SetUp();
	Renderer::SetUp();

	// main: program loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Data::Inputs.Update(window);

		// debug: detect if window should close
		if (Data::Inputs.ActionEscape)
		{
			glfwSetWindowShouldClose(window, true);
		}

		World::Update(deltaTime);
		Animator::Update(deltaTime);
		Renderer::Draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}