#include "precompiled.h"
#include "Renderer.h"


glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);

std::shared_ptr<VulkanInstance> Renderer::s_Instance;
std::shared_ptr<VulkanDevice> Renderer::s_Device;
std::shared_ptr<VulkanSwapChain> Renderer::s_SwapChain;

uint32_t Renderer::s_SwapChainWidth;
uint32_t Renderer::s_SwapChainHeight;

bool Renderer::s_ResizeRequested;

uint32_t Renderer::s_CurrentBufferIndex = 0;
uint32_t Renderer::s_CurrentImageIndex = 0;

std::vector<VkCommandBuffer> Renderer::s_CommandBuffers;
std::vector<VkSemaphore> Renderer::s_RenderCompleteSemaphores;
std::vector<VkSemaphore> Renderer::s_PresentCompleteSemaphores;
std::vector<VkFence> Renderer::s_Fences;

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
    prepareCommandBuffer();
    prepareRenderPass();

    renderScene();

    endRenderPass();
    submitCommandBuffer();
    submitFrame();
    
    s_CurrentBufferIndex = (s_CurrentBufferIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::CleanUp()
{
    vkQueueWaitIdle(s_Device->GraphicsQueue);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(s_Device->LogicalDevice, s_RenderCompleteSemaphores[i], nullptr);
        vkDestroySemaphore(s_Device->LogicalDevice, s_PresentCompleteSemaphores[i], nullptr);
        vkDestroyFence(s_Device->LogicalDevice, s_Fences[i], nullptr);
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
    s_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
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
    s_Fences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(s_Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_RenderCompleteSemaphores[i]);
        vkCreateSemaphore(s_Device->LogicalDevice, &semaphoreCreateInfo, nullptr, &s_PresentCompleteSemaphores[i]);
        vkCreateFence(s_Device->LogicalDevice, &fenceCreateInfo, nullptr, &s_Fences[i]);
    }
}

void Renderer::setClearValue(float r, float g, float b, float a, float depth)
{
    s_ClearValue.color = { r, g, b, a };
    s_ClearValue.depthStencil = { depth, 0 };
}


void Renderer::prepareFrame()
{
    vkWaitForFences(s_Device->LogicalDevice, 1, &s_Fences[s_CurrentBufferIndex], VK_TRUE, UINT64_MAX);

    // resize buffer if the window has changed size
    if (s_ResizeRequested)
    {
        WindowSize size = Window::GetSize();
        s_SwapChainWidth = size.Width;
        s_SwapChainHeight = size.Height;
        s_SwapChain->Create(s_RenderPass, &s_SwapChainWidth, &s_SwapChainHeight);
        initViewport();
        s_ResizeRequested = false;
    }

    vkResetFences(s_Device->LogicalDevice, 1, &s_Fences[s_CurrentBufferIndex]);

    // acquire the next image from the swap chain
    VkResult result = s_SwapChain->AcquireNextImage(s_PresentCompleteSemaphores[s_CurrentBufferIndex], &s_CurrentImageIndex);

    if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR))
    {
        s_ResizeRequested = true;
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Could not prepare frame!");
    }
}

void Renderer::prepareCommandBuffer()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    vkBeginCommandBuffer(s_CommandBuffers[s_CurrentBufferIndex], &beginInfo);
}

void Renderer::prepareRenderPass()
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = s_RenderPass;
    renderPassInfo.framebuffer = s_SwapChain->FrameBuffers[s_CurrentBufferIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = s_SwapChain->Extent;

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &s_ClearValue;

    vkCmdBeginRenderPass(s_CommandBuffers[s_CurrentBufferIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::renderScene()
{
    // TODO: loop through meshes and draw them
}

void Renderer::endRenderPass()
{
    vkCmdEndRenderPass(s_CommandBuffers[s_CurrentBufferIndex]);
}

void Renderer::submitCommandBuffer()
{
    // stop recording commands
    VkResult result = vkEndCommandBuffer(s_CommandBuffers[s_CurrentBufferIndex]);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_DEVICE::Failed to end command buffer recording!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &s_CommandBuffers[s_CurrentBufferIndex];
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &s_PresentCompleteSemaphores[s_CurrentBufferIndex];

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &s_RenderCompleteSemaphores[s_CurrentBufferIndex];

    // submit to the queue
    result = vkQueueSubmit(s_Device->GraphicsQueue, 1, &submitInfo, s_Fences[s_CurrentBufferIndex]);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_DEVICE::Failed to submit to queue!");
    }
}

void Renderer::submitFrame()
{
    VkResult result = s_SwapChain->QueuePresent(s_Device->GraphicsQueue, s_CurrentImageIndex, s_RenderCompleteSemaphores[s_CurrentBufferIndex]);

    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        s_ResizeRequested = true;
    }

    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Queue present failed!");
    }
}


