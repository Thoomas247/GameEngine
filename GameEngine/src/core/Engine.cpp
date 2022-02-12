#include "Engine.h"

#include "Window.h"
#include "Input.h"
#include "Log.h"

#include "../managers/ProjectManager.h"
#include "../managers/SceneManager.h"

#include "../renderer/Renderer.h"

#include "../gui/EngineGUI.h"

// debug:
#include "../importer/Importer.h"
#include "../managers/ModelManager.h"

int Engine::Run()
{
	Window::InitWindow(3200, 1800);

	ProjectManager::CreateProject("TestGame", "TestGame/");
	SceneManager::CreateScene();

	Renderer::Init();

	Entity* entity1 = SceneManager::AddEntity("Entity1");	// TODO: switch to shared_ptr structure for entities (instead of storing in a map)
	entity1->AddTransformComponent();
	entity1->AddCameraComponent(90.0f);

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