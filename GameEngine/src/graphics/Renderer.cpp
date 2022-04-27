#include "precompiled.h"
#include "Renderer.h"

#include "vulkan/VulkanRenderer.h"
#include "ECS/components/MeshComponent.h"

glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);


/* -- PUBLIC -- */

void Renderer::Draw()
{
    VulkanState::Renderer->StartRendering();
    
    for (auto& mesh : ECS::GetComponents<MeshComponent>())
    {
        VulkanState::Renderer->Submit(mesh.Shader.Pipeline.Pipeline, mesh.VertexBuffer.Buffer, mesh.IndexBuffer.Buffer, mesh.NumIndices);
    }

    VulkanState::Renderer->FinishRendering();
}

void Renderer::RequestResize()
{
    VulkanState::Renderer->RequestResize();
}
