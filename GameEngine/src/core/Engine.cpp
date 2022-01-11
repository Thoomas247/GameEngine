#include "Engine.h"

#include <iostream>

#include "Window.h"
#include "ProjectManager.h"
#include "Input.h"
#include "World.h"
#include "Log.h"

#include "../renderer/Renderer.h"

#ifdef DEV_MODE
#include "../gui/EngineGUI.h"
#endif // DEV_MODE

// debug:
#include "../game/Player.h"
#include "../renderer/Camera.h"
#include "../importer/Importer.h"
#include "ModelLoader.h"

int Engine::Run()
{
	Window::InitWindow(2200, 1300);

#ifdef DEV_MODE
	EngineGUI::Init();
#endif // DEV_MODE

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
	tree2->m_LocalPosition = glm::vec3(30.0f, 0.0f, 0.0f);
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

#ifdef DEV_MODE
		EngineGUI::Draw();
#else
		Renderer::Draw();
#endif // DEV_MODE

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();

#ifdef DEV_MODE
	EngineGUI::CleanUp();
#endif // DEV_MODE

	return 0;
}