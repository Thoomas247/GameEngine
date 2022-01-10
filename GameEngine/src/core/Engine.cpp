#include "Engine.h"

#include <iostream>

#include "Window.h"
#include "ProjectManager.h"
#include "EngineGUI.h"
#include "World.h"
#include "Input.h"

#include "../renderer/Renderer.h"

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
	///////////////////////////////////////////////////////////
	ProjectManager::CreateProject("TestGame", "C:/Users/TM1/source/repos/GameEngine/GameEngine/TestGame/");

	if (ProjectManager::g_CurrentProject == nullptr)
	{
		std::cout << "ENGINE::ERROR::No project loaded!" << std::endl;
		return -1;
	}
	///////////////////////////////////////////////////////////

	// TODO: open GUI here
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

	EngineGUI::Init();
	Renderer::Init();

	World::SetUp();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	int frameCount = 0;

	// main engine loop
	while (Window::g_IsOpen)
	{
		// detect if window should close
		if (Input::g_ActionEscape)
		{
			Window::CloseWindow();
		}

		currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();
		World::Update(deltaTime);

		Renderer::Draw();
		EngineGUI::Draw();

		Window::SwapBuffers();
		Window::PollEvents();

		frameCount += 1;
	}

	EngineGUI::CleanUp();
	Window::CleanUp();

	return 0;
}