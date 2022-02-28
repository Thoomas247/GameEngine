#pragma once

/*
* Implementation based on: https://vulkan-tutorial.com/Introduction
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

public:
	static void Init();
	static void Draw();
	static void CleanUp();

	static void RequestResize() { s_ResizeRequested = true; }

	static void SetViewMatrix(const glm::mat4& viewMatrix) { s_ViewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const glm::mat4& projectionMatrix) { s_ProjectionMatrix = projectionMatrix; }
};