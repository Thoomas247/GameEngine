#include "precompiled.h"
#include "VulkanRenderer.h"


bool VulkanRenderer::s_ResizeRequested = false;

uint32_t VulkanRenderer::s_CurrentFrameIndex = 0;
uint32_t VulkanRenderer::s_CurrentSwapChainImageIndex = 0;

std::vector<VkCommandBuffer> VulkanRenderer::s_CommandBuffers;
std::vector<VkSemaphore> VulkanRenderer::s_RenderCompleteSemaphores;
std::vector<VkSemaphore> VulkanRenderer::s_PresentCompleteSemaphores;
std::vector<VkFence> VulkanRenderer::s_RenderFences;

VkClearValue VulkanRenderer::s_ClearValue[2];


/* -- PUBLIC -- */

void VulkanRenderer::Init()
{
    VulkanState::Init();

    VulkanState::SwapChain->Create(800, 600); // pass fallback window dimensions of 800 by 600

    initCommandBuffers();
    initSemaphoresAndFences();

    setClearValue(0.05f, 0.2f, 0.5f, 1.0f, 0.0f);
}

void VulkanRenderer::StartRendering()
{
	prepareFrame();
	beginCommandBuffer();
	beginRenderPass();
}

void VulkanRenderer::Submit(VkPipeline pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer, const uint32_t& indexCount)
{
    vkCmdBindPipeline(s_CommandBuffers[s_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(s_CommandBuffers[s_CurrentFrameIndex], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(s_CommandBuffers[s_CurrentFrameIndex], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(s_CommandBuffers[s_CurrentFrameIndex], indexCount, 1, 0, 0, 0);
}

void VulkanRenderer::FinishRendering()
{
	endRenderPass();
	endCommandBuffer();
	queueSubmit();
	queuePresent();
}

void VulkanRenderer::Cleanup()
{
    vkDeviceWaitIdle(VulkanState::Device->LogicalDevice);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(VulkanState::Device->LogicalDevice, s_RenderCompleteSemaphores[i], nullptr);
        vkDestroySemaphore(VulkanState::Device->LogicalDevice, s_PresentCompleteSemaphores[i], nullptr);
        vkDestroyFence(VulkanState::Device->LogicalDevice, s_RenderFences[i], nullptr);
    }

    VulkanState::Cleanup();
}

void VulkanRenderer::RequestResize()
{
    s_ResizeRequested = true;
}


/* -- PRIVATE -- */

void VulkanRenderer::initCommandBuffers()
{
    // we need 1 command buffer per image, even though only MAX_FRAMES_IN_FLIGHT will be used at a time
    s_CommandBuffers.resize(VulkanState::SwapChain->ImageCount);
    for (int i = 0; i < VulkanState::SwapChain->ImageCount; i++)
    {
        s_CommandBuffers[i] = VulkanState::Device->CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }
}

void VulkanRenderer::initSemaphoresAndFences()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    s_RenderCompleteSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_PresentCompleteSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_RenderFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(VulkanState::Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_RenderCompleteSemaphores[i]);
        vkCreateSemaphore(VulkanState::Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_PresentCompleteSemaphores[i]);
        vkCreateFence(VulkanState::Device->LogicalDevice, &fenceCreateInfo, nullptr, &s_RenderFences[i]);
    }
}

void VulkanRenderer::setClearValue(float r, float g, float b, float a, float depth)
{
    VkClearValue colorValue{};
    colorValue.color = { r, g, b, a };

    VkClearValue depthValue{};
    depthValue.depthStencil = { 0.0f, 0 };

    s_ClearValue[0] = colorValue;
    s_ClearValue[1] = depthValue;
}

void VulkanRenderer::recreateSwapChain()
{
    // if window is minimized, wait before recreating swap chain
    WindowSize size = Window::GetSize();
    while (size.Width == 0 || size.Height == 0)
    {
        size = Window::GetSize();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(VulkanState::Device->LogicalDevice);
    VulkanState::SwapChain->Create(size.Width, size.Height);
    s_ResizeRequested = false;
}

void VulkanRenderer::prepareFrame()
{
    vkWaitForFences(VulkanState::Device->LogicalDevice, 1, &s_RenderFences[s_CurrentFrameIndex], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(VulkanState::Device->LogicalDevice, VulkanState::SwapChain->SwapChain, UINT64_MAX, s_PresentCompleteSemaphores[s_CurrentFrameIndex], VK_NULL_HANDLE, &s_CurrentSwapChainImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to acquire swap chain image!");
    }

    vkResetFences(VulkanState::Device->LogicalDevice, 1, &s_RenderFences[s_CurrentFrameIndex]);
}

void VulkanRenderer::beginCommandBuffer()
{
    vkResetCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex], /*VkCommandBufferResetFlagBits*/ 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex], &beginInfo) != VK_SUCCESS) {
        LOG_ERROR("VULKAN_RENDERER::Failed to begin recording command buffer!");
    }
}

void VulkanRenderer::beginRenderPass()
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = VulkanState::RenderPass->RenderPass;
    renderPassInfo.framebuffer = VulkanState::SwapChain->FrameBuffers[s_CurrentSwapChainImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = VulkanState::SwapChain->Extent;

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = s_ClearValue;

    vkCmdBeginRenderPass(s_CommandBuffers[s_CurrentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderer::endRenderPass()
{
    vkCmdEndRenderPass(s_CommandBuffers[s_CurrentFrameIndex]);
}

void VulkanRenderer::endCommandBuffer()
{
    if (vkEndCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex]) != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to record command buffer!");
    }
}

void VulkanRenderer::queueSubmit()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &s_PresentCompleteSemaphores[s_CurrentFrameIndex];

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &s_CommandBuffers[s_CurrentFrameIndex];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &s_RenderCompleteSemaphores[s_CurrentFrameIndex];

    if (vkQueueSubmit(VulkanState::Device->GraphicsQueue, 1, &submitInfo, s_RenderFences[s_CurrentFrameIndex]) != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to submit draw command buffer!");
    }
}

void VulkanRenderer::queuePresent()
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &s_RenderCompleteSemaphores[s_CurrentFrameIndex];

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &VulkanState::SwapChain->SwapChain;

    presentInfo.pImageIndices = &s_CurrentSwapChainImageIndex;

    VkResult result = vkQueuePresentKHR(VulkanState::Device->GraphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || s_ResizeRequested)
    {
        s_ResizeRequested = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to present swap chain image!");
    }
}
