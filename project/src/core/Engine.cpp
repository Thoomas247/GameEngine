#include "Engine.h"

#include "Window.h"
#include "World.h"
#include "Input.h"

#include "../renderer/Renderer.h"
#include "../renderer/Animator.h"

// for debug only:
#include "../importer/Importer.h"
#include "ModelLoader.h"
#include "../game/Player.h"

int Engine::Run()
{
	// init window
	Window::InitWindow();
	if (!Window::g_IsOpen)
		return -1;

	// reserve space in renderer/animator lists
	Renderer::Reserve();
	Animator::Reserve();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// This block simulates what the user would be able to do thru the GUI:

	// Importer functions called when user wants to import an asset
	// Converts the asset into a file which the engine can easily load when ModelLoader::LoadModel() is called
	//Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	// ModelLoader::LoadModel() is called when the user wants to add an asset to the world

	
	std::shared_ptr<GameObject> viking = ModelLoader::LoadModel("Viking.GEM");
	World::AddGameObject("Viking", viking);

	std::shared_ptr<GameObject> tree = ModelLoader::LoadModel("Tree.GEM");
	tree->m_LocalPosition = glm::vec3(40.0f, 0.0f, 0.0f);
	tree->m_LocalRotation = glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	tree->m_LocalScale = glm::vec3(10.0f);
	World::AddGameObject("Tree", tree);
	

	std::shared_ptr<GameObject> bird = ModelLoader::LoadModel("Bird.GEM");
	bird->m_LocalPosition = glm::vec3(0.0f, 0.0f, 120.0f);
	World::AddGameObject("Bird", bird);

	std::shared_ptr<GameObject> bird2 = ModelLoader::LoadModel("Bird.GEM");
	bird2->m_LocalPosition = glm::vec3(0.0f, 120.0f, -120.0f);
	World::AddGameObject("Bird2", bird2);

	std::shared_ptr<Player> player = std::make_shared<Player>();
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	player->AddChild("Camera", camera);
	World::AddGameObject("Player", player);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// run setup functions
	World::SetUp();
	Renderer::SetUp();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	int frameCount = 0;

	// main engine loop
	while (Window::g_IsOpen)
	{
		if (frameCount % 100 == 0)
		{
			std::shared_ptr<GameObject> tree1 = ModelLoader::LoadModel("Tree.GEM");
			tree1->m_LocalPosition = glm::vec3(frameCount / 100.0f, 0.0f, 0.0f);
			World::AddGameObject(std::to_string(frameCount / 100), tree1);

			std::shared_ptr<GameObject> tree2 = ModelLoader::LoadModel("Tree.GEM");
			tree2->m_LocalPosition = glm::vec3(0.0f, 0.0f, frameCount / 100.0f);
			World::AddGameObject(std::to_string(-frameCount / 100), tree2);
		}

		float currentFrame = float(glfwGetTime());
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