#pragma once

#include "graphics/vulkan/StructsAndConstants.h"
#include "graphics/vulkan/VulkanInstance.h"
#include "graphics/vulkan/VulkanDevice.h"
#include "graphics/vulkan/VulkanSwapChain.h"
#include "graphics/vulkan/VulkanRenderPass.h"

class VulkanState
{
public:
	static std::unique_ptr<VulkanInstance> Instance;
	static std::unique_ptr<VulkanDevice> Device;
	static std::unique_ptr<VulkanSwapChain> SwapChain;
	static std::unique_ptr<VulkanRenderPass> RenderPass;

public:
	static void Init();
	static void Cleanup();
};

