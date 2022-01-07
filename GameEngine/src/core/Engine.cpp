#include "Engine.h"

#include <iostream>

#include "Window.h"
#include "World.h"
#include "Input.h"
#include "ProjectManager.h"

#include "../renderer/Renderer.h"
#include "../renderer/Animator.h"

// debug:
#include "../game/Player.h"
#include "../renderer/Camera.h"
#include "../importer/Importer.h"
#include "ModelLoader.h"

int Engine::Run()
{
	// init window
	Window::InitWindow();
	if (!Window::g_IsOpen)
		return -1;

	// TODO: create menu for engine to create/load project
	// debug: create project
	ProjectManager::CreateProject("TestGame", "C:/Users/TM1/source/repos/GameEngine/GameEngine/TestGame/");

	if (ProjectManager::g_CurrentProject == nullptr)
	{
		std::cout << "ENGINE::ERROR::No project loaded!" << std::endl;
		return -1;
	}

	// reserve space in renderer/animator lists
	Renderer::Reserve();
	Animator::Reserve();

	// TODO: open GUI here
	// debug:
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");
	std::shared_ptr<GameObject> tree = ModelLoader::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
	World::AddGameObject("Tree", tree);

	auto player = std::make_shared<Player>();
	auto camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	World::AddGameObject("Player", player);

	// run setup functions
	World::SetUp();
	Renderer::SetUp();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	int frameCount = 0;

	// main engine loop
	while (Window::g_IsOpen)
	{
		currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();

		// detect if window should close
		if (Input::g_ActionEscape)
		{
			Window::CloseWindow();
		}

		World::Update(deltaTime);
		Animator::Update(deltaTime);
		Renderer::Draw();
		Window::SwapBuffers();
		Window::PollEvents();

		frameCount += 1;
	}

	return 0;
}