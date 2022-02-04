#include "Engine.h"

#include "Window.h"
#include "Input.h"
#include "Log.h"

#include "../managers/SceneManager.h"
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

	// TODO: create menu for engine to create/load project, fix ProjectManager to match SceneManager structure
	ProjectManager::CreateProject("TestGame", "TestGame/");

	// TODO: make this possible in the GUI, finish Scene class
	Scene mainScene = Scene("Main", "");
	SceneManager::SetScene(&mainScene);

	TextureManager::Init();
	EngineGUI::Init();
	Renderer::Init();

	// TODO: make all the operations below possible to do in the GUI
	///////////////////////////////////////////////////////////

	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	std::shared_ptr<GameObject> bird = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Bird.GEM");
	bird->LocalPosition = glm::vec3(50.0f, 40.0f, 50.0f);
	bird->LocalScale = glm::vec3(0.5f);
	SceneManager::AddGameObjectToScene(bird);

	// create 100 trees in a 10 * 10 grid (for debugging)
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			auto tree = ModelManager::LoadModel(ProjectManager::GetModelsPath() + "Tree.GEM");
			//tree->SetName(std::to_string(x) + ", " + std::to_string(z));
			tree->LocalPosition = glm::vec3(10 * x, 0.0f, 10 * z);
			tree->SetLocalRotationFromEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
			SceneManager::AddGameObjectToScene(tree);
		}
	}

	///////////////////////////////////////////////////////////

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

	SceneManager::SetUpScene();

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

		Input::Update();
		SceneManager::UpdateScene(deltaTime);
		EngineGUI::Update(deltaTime);
		//Renderer::Draw(); // not needed here in "engine" mode

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();
	EngineGUI::CleanUp();

	return 0;
}