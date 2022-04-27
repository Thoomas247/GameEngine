#include "precompiled.h"
#include "Engine.h"

// debug:
#include "../ECS/components/TransformComponent.h"
#include "../ECS/components/MeshComponent.h"
#include "../ECS/components/CameraComponent.h"

int Engine::Run()
{
	Window::InitWindow(800, 600);
	VulkanState::Init();
	//GraphicsAssetManager::Init();

	ProjectManager::CreateProject("TestGame", "TestGame/");
	//SceneManager::CreateScene();

	// TODO:
	// [x] make viewport and scissor size dynamic in vulkan
	// [x] find a better way to clean up vulkan objects on application close
	// [] fix importing to match new format:
	//		[] import into .scene file which contains each component type in separate lists
	//		[] create skeleton component
	//		[] save skeleton in new format
	//		[] save animations in new format
	//		[] fix animation linear interpolation on import
	// [] implement camera component properly
	// [] fix scene loading to match new format
	// [] add scene saving
	// [] add skeleton loading to scene loading
	// [] create texture loader
	// [x] make shaders only require 1 file
	// [] find shader uniform locations on load
	// [] create animator


	//Importer::Import("F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");
	SceneManager::LoadScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");
	//SceneManager::LoadSubScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");

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

		//LOG_INFO(std::to_string(1 / deltaTime));

		Input::Update();
		//SceneManager::Update(deltaTime);

		Renderer::Draw();

		Window::SwapBuffers();
		Window::PollEvents();
	}

	VulkanState::WaitIdle();
	Window::CleanUp();

	return 0;
}