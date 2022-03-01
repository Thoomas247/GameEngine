#pragma once

/*
* Implementation based on: 
*	https://vulkan-tutorial.com/Introduction
*	https://github.com/SaschaWillems/Vulkan/tree/master/base
*/

#include "graphics/vulkan/StructsAndConstants.h"
#include "graphics/vulkan/VulkanInstance.h"
#include "graphics/vulkan/VulkanDevice.h"
#include "graphics/vulkan/VulkanSwapChain.h"

class Renderer
{
private:
	static glm::mat4 s_ViewMatrix;
	static glm::mat4 s_ProjectionMatrix;

	static std::shared_ptr<VulkanInstance> s_Instance;
	static std::shared_ptr<VulkanDevice> s_Device;
	static std::shared_ptr<VulkanSwapChain> s_SwapChain;

	static uint32_t s_SwapChainWidth;
	static uint32_t s_SwapChainHeight;

	static bool s_ResizeRequested;

	static uint32_t s_CurrentBufferIndex;
	static uint32_t s_CurrentImageIndex;

	static std::vector<VkCommandBuffer> s_CommandBuffers;
	static std::vector<VkSemaphore> s_RenderCompleteSemaphores;
	static std::vector<VkSemaphore> s_PresentCompleteSemaphores;
	static std::vector<VkFence> s_Fences;

	static VkViewport s_Viewport;

	static VkRenderPass s_RenderPass;

	static VkClearValue s_ClearValue;

public:
	static void Init();
	static void Draw();
	static void CleanUp();

	static void RequestResize() { s_ResizeRequested = true; }

	static void SetViewMatrix(const glm::mat4& viewMatrix) { s_ViewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const glm::mat4& projectionMatrix) { s_ProjectionMatrix = projectionMatrix; }

private:
	static void initRenderPass();
	static void initViewport();
	static void initCommandBuffers();
	static void initSemaphoresAndFences();
	static void setClearValue(float r, float g, float b, float a, float depth);

	static void prepareFrame();
	static void prepareCommandBuffer();
	static void prepareRenderPass();
	static void renderScene();
	static void endRenderPass();
	static void submitCommandBuffer();
	static void submitFrame();
	
};