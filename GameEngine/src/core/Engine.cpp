#include "precompiled.h"
#include "Engine.h"

// debug:
#include "../ECS/components/TransformComponent.h"
#include "../ECS/components/MeshComponent.h"
#include "../ECS/components/CameraComponent.h"

int Engine::Run()
{
	Window::InitWindow(800, 600);
	//GraphicsAssetManager::Init();
	Renderer::Init();

	ProjectManager::CreateProject("TestGame", "TestGame/");
	//SceneManager::CreateScene();

	// TODO:
	// [x] clean up header files and create cpp files where needed (especially entity class)
	// [x] implement camera component properly
	// [] fix importing to match new format:
	//		[x] import into .scene file which contains each component type in separate lists
	//		[] create skeleton component
	//		[] save skeleton in new format
	//		[] save animations in new format
	//		[] fix animation linear interpolation on import
	// [x] fix scene loading to match new format
	// [] add scene saving
	// [] add skeleton loading to scene loading
	// [x] create asset manager to manage GPU assets:
	//		[x] Textures
	//		[x] Shaders
	//		[x] VAOs
	//		[x] Finish ShaderMaterial to replace shader and material
	// [x] make shaders only require 1 file
	// [] find shader uniform locations on load
	// [] create animator
	// [] (maybe) remove entity pointer in Component class and store a copy of the m_Components array in each component (higher mem usage, slower entity deletion/moving, faster access?)
	//		since TransformComponents keep track of their parent the hierarchy will be maintained


	//Importer::Import("F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");
	//SceneManager::LoadScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");
	//SceneManager::LoadSubScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");

	ECS::CreateComponent<MeshComponent>(nullptr);

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

	Renderer::CleanUp();
	Window::CleanUp();

	return 0;
}