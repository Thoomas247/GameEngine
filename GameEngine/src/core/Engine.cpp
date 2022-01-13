#include "Engine.h"

#include <iostream>

#include "Window.h"
#include "ProjectManager.h"
#include "Input.h"
#include "World.h"
#include "Log.h"

#include "../renderer/Renderer.h"

#include "../gui/EngineGUI.h"

// debug:
#include "../game/Player.h"
#include "../renderer/Camera.h"
#include "../importer/Importer.h"
#include "ModelLoader.h"

int Engine::Run()
{
	Window::InitWindow(2200, 1300);
	EngineGUI::Init();
	Renderer::Init();

	// TODO: create menu for engine to create/load project
	ProjectManager::CreateProject("TestGame", "C:/Users/TM1/source/repos/GameEngine/GameEngine/TestGame/");

	if (ProjectManager::CurrentProject == nullptr)
	{
		LOG_ERROR("ENGINE::No project loaded!")
		return -1;
	}

	///////////////////////////////////////////////////////////
	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	std::shared_ptr<GameObject> tree = ModelLoader::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
	World::AddGameObject("Tree", tree);

	std::shared_ptr<GameObject> tree2 = ModelLoader::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
	tree2->LocalPosition = glm::vec3(30.0f, 0.0f, 0.0f);
	World::AddGameObject("Tree2", tree2);

	std::shared_ptr<GameObject> bird = ModelLoader::LoadModel(ProjectManager::GetModelsPath() + "Bird.GEM");
	World::AddGameObject("Bird", bird);

	auto player = std::make_shared<Player>();
	auto camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	World::AddGameObject("Player", player);
	///////////////////////////////////////////////////////////

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	// TODO: Separate engine loop from game loop (use Game namespace)
	World::SetUp();

	// main engine loop
	while (!Window::ShouldClose)
	{
		// detect if window should close
		if (Input::ActionEscape)
		{
			Window::CloseWindow();
			continue;	// window might not close right away
		}

		currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();
		World::Update(deltaTime);

		EngineGUI::Draw();
		//Renderer::Draw(); // not needed here in engine mode

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();
	EngineGUI::CleanUp();

	return 0;
}