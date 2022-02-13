#include "Engine.h"

#include "Window.h"
#include "Input.h"
#include "Log.h"

#include "../managers/ProjectManager.h"
#include "../managers/SceneManager.h"

#include "../importer/GLTFImporter.h"

#include "../graphics/GraphicsAssetManager.h"

#include "../renderer/Renderer.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);
	GraphicsAssetManager::Init();
	Renderer::Init();

	ProjectManager::CreateProject("TestGame", "TestGame/");
	SceneManager::CreateScene();

	// TODO:
	// [] fix importing to match new format:
	//		 [x] import into .scene file which contains each component type in separate lists
	//		 [] create skeleton component
	//		 [] save skeleton in new format
	//		 [] save animations in new format
	//		 [] fix animation linear interpolation on import
	// [] fix model loading to match new format
	// [x] create asset manager to manage GPU assets:
	//		[x] Textures
	//		[x] Shaders
	//		[x] VAOs
	// [] make shaders only require 1 file
	// [] find shader uniform locations on load
	// [] create animator

	GLTFImporter::Import("F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");

	for (int i = 0; i < 1000; i++)
	{
		auto entity = SceneManager::AddEntity("Entity");
		entity->AddTransformComponent();
		entity->AddMeshComponent();
		entity->AddCameraComponent(90.0f);
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