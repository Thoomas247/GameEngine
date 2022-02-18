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
#include "../ECS/components/CameraComponent.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);
	GraphicsAssetManager::Init();
	Renderer::Init();

	ProjectManager::CreateProject("TestGame", "TestGame/");
	SceneManager::CreateScene();

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
	// [] (maybe) remove entity pointer in Component class and store a copy of the m_Components array in each component (higher mem usage, slower entity deletion/moving, faster access?)
	//		since TransformComponents keep track of their parent the hierarchy will be maintained

	//GLTFImporter::Import("F:/Users/TM1/Downloads/Tree/MyFirstTree.gltf");
	SceneManager::LoadScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");
	SceneManager::LoadSubScene(ProjectManager::GetScenesPath() + "MyFirstTree.scene");

	// testing: //
	auto cameraOrbiter = SceneManager::CreateEntityAtRoot("Orbiter");
	cameraOrbiter->AddComponent<TransformComponent>();
	auto testCamera = cameraOrbiter->CreateChild("Camera");
	testCamera->AddComponent<TransformComponent>(glm::vec3(0.0f, 30.0f, 40.0f), glm::quat(glm::vec3(-0.5f, 0.0f, 0.0f)));
	testCamera->AddComponent<CameraComponent>();
	testCamera->GetComponent<CameraComponent>().SetActive(true);

	float yaw = 0.0f;
	/////////////


	/*
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

		// for fun: //
		yaw += deltaTime;
		cameraOrbiter->GetComponent<TransformComponent>().SetLocalRotation(glm::quat(glm::vec3(0.0f, yaw, 0.0f)));
		/////////////

		Input::Update();
		SceneManager::Update(deltaTime);

		Renderer::Draw();

		Window::SwapBuffers();
		Window::PollEvents();
	}

	Window::CleanUp();

	return 0;
}