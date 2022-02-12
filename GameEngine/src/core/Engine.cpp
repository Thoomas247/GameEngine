#include "Engine.h"

#include "Window.h"
#include "Input.h"
#include "Log.h"

#include "../managers/ProjectManager.h"
#include "../managers/TextureManager.h"

#include "../renderer/Renderer.h"

#include "../gui/EngineGUI.h"

// debug:
#include "../renderer/Camera.h"
#include "../importer/Importer.h"
#include "../managers/ModelManager.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);

	ProjectManager::CreateProject("TestGame", "TestGame/");

	Renderer::Init();

	/*
	for (int i = 0; i < 1000; i++)
	{
		uint64_t id = mainScene.CreateEntity("Test" + std::to_string(i));
		mainScene.CreateMeshComponent(id, VertexArray(), Shader(), Material());
	}
	*/

	//TextureManager::Init();
	//EngineGUI::Init();

	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	/*
	std::shared_ptr<GameObject> bird = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Bird.GEM");	// possible
	bird->LocalPosition = glm::vec3(50.0f, 40.0f, 50.0f);	// possible
	bird->LocalScale = glm::vec3(0.5f);	// possible
	SceneManager::AddGameObjectToScene(bird);	// possible

	// create 100 trees in a 10 * 10 grid (for debugging)
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			auto tree = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");	// possible
			//tree->SetName(std::to_string(x) + ", " + std::to_string(z));
			tree->LocalPosition = glm::vec3(10 * x, 0.0f, 10 * z);	// possible
			tree->SetLocalRotationFromEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));	// possible
			SceneManager::AddGameObjectToScene(tree);	// possible
		}
	}
	*/

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	// main engine loop
	while (!Window::ShouldClose())
	{
		// detect if window should close
		if (Input::GetActionEscape())
		{
			Window::CloseWindow();
			continue;
		}

		currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		LOG_INFO(std::to_string(1 / deltaTime));

		Input::Update();

		Renderer::Draw();

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();
	//EngineGUI::CleanUp();

	return 0;
}