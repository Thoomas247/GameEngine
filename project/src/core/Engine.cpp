#include "Engine.h"

#include "Window.h"
#include "World.h"
#include "Input.h"

#include "../renderer/Renderer.h"
#include "../renderer/Animator.h"

// for debug only:
#include "../importer/Importer.h"
#include "ModelLoader.h"
#include "../game/Player.h";

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
	//Importer::ImportGLTF("Bird", "F:/Users/TM1/Downloads/phoenix_bird/scene.gltf");
	//Importer::ImportGLTF("Viking", "F:/Users/TM1/Downloads/viking_room/scene.gltf");
	//Importer::ImportGLTF("Snowfox", "C:/Users/Thomas/Downloads/Snowfox/scene.gltf");
	Importer::ImportGLTF("Tree", "F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	// ModelLoader::LoadModel() is called when the user wants to add an asset to the game world
	//std::shared_ptr<GameObject> viking = ModelLoader::LoadModel("Viking.GEM");
	//World::AddGameObject("Viking", viking);

	std::shared_ptr<GameObject> tree = ModelLoader::LoadModel("Tree.GEM");
	//tree->m_LocalScale = glm::vec3(0.01f, 0.01f, 0.01f);
	World::AddGameObject("Tree", tree);

	//std::shared_ptr<GameObject> bird = ModelLoader::LoadModel("Bird.GEM");
	//World::AddGameObject("Bird", bird);

	//std::shared_ptr<GameObject> snowfox = ModelLoader::LoadModel("Snowfox.GEM");
	//World::AddGameObject("Snowfox", snowfox);

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

	// main engine loop
	while (Window::g_IsOpen)
	{
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
	}

	return 0;
}