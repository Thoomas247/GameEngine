#include "Engine.h"

#include <iostream>

#include "Window.h"
#include "Input.h"
#include "World.h"
#include "Log.h"

#include "../managers/ProjectManager.h"
#include "../managers/TextureManager.h"

#include "../renderer/Renderer.h"

#include "../gui/EngineGUI.h"

// debug:
#include "../game/Player.h"
#include "../renderer/Camera.h"
#include "../importer/Importer.h"
#include "../managers/ModelManager.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);

	// TODO: create menu for engine to create/load project
	ProjectManager::CreateProject("TestGame", "C:/Users/TM1/source/repos/GameEngine/GameEngine/TestGame/");

	if (ProjectManager::CurrentProject == nullptr)
	{
		LOG_ERROR("ENGINE::No project loaded!")
		return -1;
	}

	TextureManager::Init();

	EngineGUI::Init();
	Renderer::Init();


	// TODO: make all the operations below possible to do in the GUI
	///////////////////////////////////////////////////////////
	
	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	std::shared_ptr<GameObject> tree = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
	World::AddGameObject("Tree", tree);

	std::shared_ptr<GameObject> tree2 = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
	tree2->SetLocalPosition(glm::vec3(30.0f, 0.0f, 0.0f));
	World::AddGameObject("Tree2", tree2);

	std::shared_ptr<GameObject> bird = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Bird.GEM");
	World::AddGameObject("Bird", bird);

	auto player = std::make_shared<Player>();
	auto camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	World::AddGameObject("Player", player);

	///////////////////////////////////////////////////////////

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	World::SetUp();

	// main engine loop
	while (!Window::ShouldClose)
	{
		// detect if window should close
		if (Input::ActionEscape)
		{
			Window::CloseWindow();
			continue;
		}

		currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();
		World::Update(deltaTime);
		EngineGUI::Update(deltaTime);
		//Renderer::Draw(); // not needed here in "engine" mode

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();
	EngineGUI::CleanUp();

	return 0;
}