#include "precompiled.h"
#include "Renderer.h"


glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);

std::shared_ptr<VulkanInstance> Renderer::s_Instance;
std::shared_ptr<VulkanDevice> Renderer::s_Device;
std::shared_ptr<VulkanSwapChain> Renderer::s_SwapChain;

uint32_t Renderer::s_SwapChainWidth;
uint32_t Renderer::s_SwapChainHeight;

bool Renderer::s_ResizeRequested = false;

uint32_t Renderer::s_CurrentFrameIndex = 0;
uint32_t Renderer::s_CurrentSwapChainImageIndex = 0;

std::vector<VkCommandBuffer> Renderer::s_CommandBuffers;
std::vector<VkSemaphore> Renderer::s_RenderCompleteSemaphores;
std::vector<VkSemaphore> Renderer::s_PresentCompleteSemaphores;
std::vector<VkFence> Renderer::s_RenderFences;

VkViewport Renderer::s_Viewport;

VkRenderPass Renderer::s_RenderPass;

VkClearValue Renderer::s_ClearValue;


/* -- PUBLIC -- */

void Renderer::Init()
{
    s_Instance = std::make_shared<VulkanInstance>();
    VkPhysicalDevice physicalDevice = s_Instance->PickPhysicalDevice();

    s_Device = std::make_shared<VulkanDevice>(physicalDevice, VkPhysicalDeviceFeatures(), VULKAN_DEVICE_EXTENSIONS);

    s_SwapChain = std::make_shared<VulkanSwapChain>(s_Instance, s_Device);
    initRenderPass();
    s_SwapChain->Create(s_RenderPass, &s_SwapChainWidth, &s_SwapChainHeight);

    initViewport();
    initCommandBuffers();
    initSemaphoresAndFences();

    setClearValue(0.05f, 0.05f, 0.05f, 1.0f, 0.0f);
}

void Renderer::Draw()
{
    prepareFrame();
    beginCommandBuffer();
    beginRenderPass();

    renderScene();

    endRenderPass();
    endCommandBuffer();
    queueSubmit();
    queuePresent();
    s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}

void Renderer::CleanUp()
{
    vkDeviceWaitIdle(s_Device->LogicalDevice);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(s_Device->LogicalDevice, s_RenderCompleteSemaphores[i], nullptr);
        vkDestroySemaphore(s_Device->LogicalDevice, s_PresentCompleteSemaphores[i], nullptr);
        vkDestroyFence(s_Device->LogicalDevice, s_RenderFences[i], nullptr);
    }
    vkDestroyRenderPass(s_Device->LogicalDevice, s_RenderPass, nullptr);

    s_SwapChain->Cleanup();
    s_Device->Cleanup();
    s_Instance->Cleanup();
}


/* -- PRIVATE -- */

void Renderer::initRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = s_SwapChain->ColorFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(s_Device->LogicalDevice, &renderPassInfo, nullptr, &s_RenderPass) != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::failed to create render pass!");
    }
}

void Renderer::initViewport()
{
    const float viewportWidth{ static_cast<float>(s_SwapChain->Extent.width) };
    const float viewportHeight{ static_cast<float>(s_SwapChain->Extent.height) };

    s_Viewport.width = viewportWidth;
    s_Viewport.height = viewportHeight;
    s_Viewport.x = 0;
    s_Viewport.y = 0;
    s_Viewport.maxDepth = 1.0f;
    s_Viewport.minDepth = 0.0f;
}

void Renderer::initCommandBuffers()
{
    // we need 1 comman buffer per image, even though only MAX_FRAMES_IN_FLIGHT will be used at a time
    s_CommandBuffers.resize(s_SwapChain->ImageCount);
    for (int i = 0; i < s_SwapChain->ImageCount; i++)
    {
        s_CommandBuffers[i] = s_Device->CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }
}

void Renderer::initSemaphoresAndFences()
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
        vkCreateSemaphore(s_Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_RenderCompleteSemaphores[i]);
        vkCreateSemaphore(s_Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_PresentCompleteSemaphores[i]);
        vkCreateFence(s_Device->LogicalDevice, &fenceCreateInfo, nullptr, &s_RenderFences[i]);
    }
}

void Renderer::setClearValue(float r, float g, float b, float a, float depth)
{
    s_ClearValue = { r, g, b, a};
}

void Renderer::recreateSwapChain()
{
    // if window is minimized, wait before recreating swap chain
    WindowSize size = Window::GetSize();
    while (size.Width == 0 || size.Height == 0) 
    {
        size = Window::GetSize();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(s_Device->LogicalDevice);
    s_SwapChainWidth = size.Width;
    s_SwapChainHeight = size.Height;
    s_SwapChain->Create(s_RenderPass, &s_SwapChainWidth, &s_SwapChainHeight);
    initViewport();
    s_ResizeRequested = false;
}


void Renderer::prepareFrame()
{
    vkWaitForFences(s_Device->LogicalDevice, 1, &s_RenderFences[s_CurrentFrameIndex], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(s_Device->LogicalDevice, s_SwapChain->SwapChain, UINT64_MAX, s_PresentCompleteSemaphores[s_CurrentFrameIndex], VK_NULL_HANDLE, &s_CurrentSwapChainImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
    {
        LOG_ERROR("RENDERER::Failed to acquire swap chain image!");
    }

    vkResetFences(s_Device->LogicalDevice, 1, &s_RenderFences[s_CurrentFrameIndex]);
}

void Renderer::beginCommandBuffer()
{
    vkResetCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex], /*VkCommandBufferResetFlagBits*/ 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex], &beginInfo) != VK_SUCCESS) {
        LOG_ERROR("RENDERER::Failed to begin recording command buffer!");
    }
}

void Renderer::beginRenderPass()
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = s_RenderPass;
    renderPassInfo.framebuffer = s_SwapChain->FrameBuffers[s_CurrentSwapChainImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = s_SwapChain->Extent;

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &s_ClearValue;

    vkCmdBeginRenderPass(s_CommandBuffers[s_CurrentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::renderScene()
{
    // TODO: loop through meshes and draw them
    //vkCmdBindPipeline(s_CommandBuffers[s_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    //vkCmdDraw(s_CommandBuffers[s_CurrentFrameIndex], 3, 1, 0, 0);
}

void Renderer::endRenderPass()
{
    vkCmdEndRenderPass(s_CommandBuffers[s_CurrentFrameIndex]);
}

void Renderer::endCommandBuffer()
{
    if (vkEndCommandBuffer(s_CommandBuffers[s_CurrentFrameIndex]) != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Failed to record command buffer!");
    }
}

void Renderer::queueSubmit()
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

    if (vkQueueSubmit(s_Device->GraphicsQueue, 1, &submitInfo, s_RenderFences[s_CurrentFrameIndex]) != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to submit draw command buffer!");
    }
}

void Renderer::queuePresent()
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &s_RenderCompleteSemaphores[s_CurrentFrameIndex];

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &s_SwapChain->SwapChain;

    presentInfo.pImageIndices = &s_CurrentSwapChainImageIndex;

    VkResult result = vkQueuePresentKHR(s_Device->GraphicsQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || s_ResizeRequested)
    {
        s_ResizeRequested = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Failed to present swap chain image!");
    }
}
