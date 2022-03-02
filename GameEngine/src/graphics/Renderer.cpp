#include "precompiled.h"
#include "Renderer.h"

#include "vulkan/VulkanRenderer.h"
#include "ECS/components/MeshComponent.h"

glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);


/* -- PUBLIC -- */

void Renderer::Init()
{
    VulkanRenderer::Init();
}

void Renderer::Draw()
{
    VulkanRenderer::StartRendering();
    
    for (auto& mesh : ECS::GetComponents<MeshComponent>())
    {
        VulkanRenderer::Submit(mesh.Pipeline.Pipeline, mesh.MeshData.VertexBuffer, mesh.MeshData.IndexBuffer, mesh.MeshData.NumIndices);
    }

    VulkanRenderer::FinishRendering();
}

void Renderer::CleanUp()
{
    VulkanRenderer::Cleanup();
}

void Renderer::RequestResize()
{
    VulkanRenderer::RequestResize();
}
