#include "Engine.h"

#include "Window.h"
#include "Input.h"
#include "Log.h"

#include "../project/ProjectManager.h"
#include "../scene/SceneManager.h"
#include "../importer/GLTFImporter.h"
#include "../graphics/GraphicsAssetManager.h"
#include "../graphics/Renderer.h"

// debug:
#include "../ECS/components/TransformComponent.h"
#include "../ECS/components/MeshComponent.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);
	GraphicsAssetManager::Init();
	Renderer::Init();

	ProjectManager::CreateProject("TestGame", "TestGame/");
	SceneManager::CreateScene();

	// TODO:
	// [] clean up header files and create cpp files where needed (especially entity class)
	// [] implement camera component properly
	// [] fix importing to match new format:
	//		[x] import into .scene file which contains each component type in separate lists
	//		[] create skeleton component
	//		[] save skeleton in new format
	//		[] save animations in new format
	//		[] fix animation linear interpolation on import
	// [x] fix scene loading to match new format
	// [] add scene saving to file
	// [] add skeleton loading to scene loading
	// [x] create asset manager to manage GPU assets:
	//		[x] Textures
	//		[x] Shaders
	//		[x] VAOs
	//		[] Make material a GPU asset (buffer) which can be set depending on an ID, instead of just being a part of the mesh component
	// [] make shaders only require 1 file
	// [] find shader uniform locations on load
	// [] create animator

	//GLTFImporter::Import("F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");
	SceneManager::LoadScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");
	SceneManager::LoadSubScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");

	
	auto ent = SceneManager::CreateEntityAtRoot("test");
	ent->AddComponent<TransformComponent>();
	ent->AddComponent<MeshComponent>();

	for (int i = 0; i < 10; i++)
	{
		auto child = ent->CreateChild("test");
		child->AddComponent<TransformComponent>();

		for (int j = 0; j < 10; j++)
		{
			auto child2 = child->CreateChild("test");
			child2->AddComponent<TransformComponent>();
		}
	}
	

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
		SceneManager::Update(deltaTime);

		Renderer::Draw();

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();
	//EngineGUI::CleanUp();

	return 0;
}