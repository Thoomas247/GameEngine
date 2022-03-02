#pragma once

#include "graphics/vulkan/StructsAndConstants.h"
#include "graphics/vulkan/VulkanInstance.h"
#include "graphics/vulkan/VulkanDevice.h"
#include "graphics/vulkan/VulkanSwapChain.h"

class VulkanRenderer
{
private:
	static std::shared_ptr<VulkanInstance> s_Instance;
	static std::shared_ptr<VulkanDevice> s_Device;
	static std::shared_ptr<VulkanSwapChain> s_SwapChain;

	static uint32_t s_SwapChainWidth;
	static uint32_t s_SwapChainHeight;

	static bool s_ResizeRequested;

	static uint32_t s_CurrentFrameIndex;
	static uint32_t s_CurrentSwapChainImageIndex;

	static std::vector<VkCommandBuffer> s_CommandBuffers;
	static std::vector<VkSemaphore> s_RenderCompleteSemaphores;
	static std::vector<VkSemaphore> s_PresentCompleteSemaphores;
	static std::vector<VkFence> s_RenderFences;

	static VkViewport s_Viewport;
	static VkRect2D s_Scissor;

	static VkRenderPass s_RenderPass;

	static VkClearValue s_ClearValue[2];

public:
	static void Init();
	static void StartRendering();
	static void Submit();
	static void FinishRendering();
	static void Cleanup();
	static void RequestResize();

	// TODO: move these to a different class
	static VkDevice GetDevice() { return s_Device->LogicalDevice; }
	static VkViewport* GetViewport() { return &s_Viewport; }
	static VkRect2D* GetScissor() { return &s_Scissor; }
	static VkRenderPass GetRenderPass() { return s_RenderPass; }

private:
	static void initRenderPass();
	static void initViewportAndScissor();
	static void initCommandBuffers();
	static void initSemaphoresAndFences();
	static void setClearValue(float r, float g, float b, float a, float depth);

	static void recreateSwapChain();

	static void prepareFrame();
	static void beginCommandBuffer();
	static void beginRenderPass();

	static void endRenderPass();
	static void endCommandBuffer();
	static void queueSubmit();
	static void queuePresent();

};

