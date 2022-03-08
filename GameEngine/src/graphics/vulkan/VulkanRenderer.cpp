#include "precompiled.h"
#include "VulkanRenderer.h"


/* -- PUBLIC -- */

VulkanRenderer::VulkanRenderer()
{
    m_Instance = VulkanState::Instance;
    m_Device = VulkanState::Device;

    m_ResizeRequested = false;
    m_CurrentFrameIndex = 0;
    m_CurrentSwapChainImageIndex = 0;

    SwapChain = std::make_unique<VulkanSwapChain>();
    RenderPass = std::make_unique<VulkanRenderPass>(SwapChain->ColorFormat);

    SwapChain->Create(RenderPass->RenderPass);

    initCommandBuffers();
    initSemaphoresAndFences();

    setClearValue(0.05f, 0.2f, 0.5f, 1.0f, 0.0f);
}

VulkanRenderer::~VulkanRenderer()
{
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(VulkanState::Device->LogicalDevice, m_RenderCompleteSemaphores[i], nullptr);
        vkDestroySemaphore(VulkanState::Device->LogicalDevice, m_PresentCompleteSemaphores[i], nullptr);
        vkDestroyFence(VulkanState::Device->LogicalDevice, m_RenderFences[i], nullptr);
    }
}

void VulkanRenderer::StartRendering()
{
	prepareFrame();
	beginCommandBuffer();
	beginRenderPass();

    vkCmdSetViewport(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &SwapChain->Viewport);
    vkCmdSetScissor(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &SwapChain->Scissor);
}

void VulkanRenderer::Submit(VkPipeline pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer, const uint32_t& indexCount)
{
    vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(m_CommandBuffers[m_CurrentFrameIndex], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(m_CommandBuffers[m_CurrentFrameIndex], indexCount, 1, 0, 0, 0);
}

void VulkanRenderer::FinishRendering()
{
	endRenderPass();
	endCommandBuffer();
	queueSubmit();
	queuePresent();
}

void VulkanRenderer::RequestResize()
{
    m_ResizeRequested = true;
}


/* -- PRIVATE -- */

void VulkanRenderer::initCommandBuffers()
{
    // we need 1 command buffer per image, even though only MAX_FRAMES_IN_FLIGHT will be used at a time
    m_CommandBuffers.resize(SwapChain->ImageCount);
    for (uint32_t i = 0; i < SwapChain->ImageCount; i++)
    {
        m_CommandBuffers[i] = VulkanState::Device->CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }
}

void VulkanRenderer::initSemaphoresAndFences()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    m_RenderCompleteSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_PresentCompleteSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_RenderFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(VulkanState::Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderCompleteSemaphores[i]);
        vkCreateSemaphore(VulkanState::Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &m_PresentCompleteSemaphores[i]);
        vkCreateFence(VulkanState::Device->LogicalDevice, &fenceCreateInfo, nullptr, &m_RenderFences[i]);
    }
}

void VulkanRenderer::setClearValue(float r, float g, float b, float a, float depth)
{
    VkClearValue colorValue{};
    colorValue.color = { r, g, b, a };

    VkClearValue depthValue{};
    depthValue.depthStencil = { depth, 0 };

    m_ClearValue[0] = colorValue;
    m_ClearValue[1] = depthValue;
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
    SwapChain->Recreate(size.Width, size.Height);
    m_ResizeRequested = false;
}

void VulkanRenderer::prepareFrame()
{
    vkWaitForFences(VulkanState::Device->LogicalDevice, 1, &m_RenderFences[m_CurrentFrameIndex], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(VulkanState::Device->LogicalDevice, SwapChain->SwapChain, UINT64_MAX, m_PresentCompleteSemaphores[m_CurrentFrameIndex], VK_NULL_HANDLE, &m_CurrentSwapChainImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to acquire swap chain image!");
    }

    vkResetFences(VulkanState::Device->LogicalDevice, 1, &m_RenderFences[m_CurrentFrameIndex]);
}

void VulkanRenderer::beginCommandBuffer()
{
    vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], /*VkCommandBufferResetFlagBits*/ 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex], &beginInfo) != VK_SUCCESS) {
        LOG_ERROR("VULKAN_RENDERER::Failed to begin recording command buffer!");
    }
}

void VulkanRenderer::beginRenderPass()
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass->RenderPass;
    renderPassInfo.framebuffer = SwapChain->FrameBuffers[m_CurrentSwapChainImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = SwapChain->Extent;

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = m_ClearValue;

    vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderer::endRenderPass()
{
    vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrameIndex]);
}

void VulkanRenderer::endCommandBuffer()
{
    if (vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrameIndex]) != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to record command buffer!");
    }
}

void VulkanRenderer::queueSubmit()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_PresentCompleteSemaphores[m_CurrentFrameIndex];

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrameIndex];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_RenderCompleteSemaphores[m_CurrentFrameIndex];

    if (vkQueueSubmit(VulkanState::Device->GraphicsQueue, 1, &submitInfo, m_RenderFences[m_CurrentFrameIndex]) != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to submit draw command buffer!");
    }
}

void VulkanRenderer::queuePresent()
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_RenderCompleteSemaphores[m_CurrentFrameIndex];

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &SwapChain->SwapChain;

    presentInfo.pImageIndices = &m_CurrentSwapChainImageIndex;

    VkResult result = vkQueuePresentKHR(VulkanState::Device->GraphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_ResizeRequested)
    {
        m_ResizeRequested = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_RENDERER::Failed to present swap chain image!");
    }
}
