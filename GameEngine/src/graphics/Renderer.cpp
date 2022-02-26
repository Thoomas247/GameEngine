#include "precompiled.h"
#include "Renderer.h"

#include "../ECS/ECS.h"
#include "../ECS/components/MeshComponent.h"
#include "../ECS/components/TransformComponent.h"

glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);


/* -- PUBLIC -- */

void Renderer::Init()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported\n";
}

void Renderer::Draw()
{
	
}