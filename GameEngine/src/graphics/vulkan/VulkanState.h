#pragma once

#include "graphics/vulkan/StructsAndConstants.h"
#include "graphics/vulkan/VulkanInstance.h"
#include "graphics/vulkan/VulkanDevice.h"
#include "graphics/vulkan/VulkanSwapChain.h"
#include "graphics/vulkan/VulkanRenderPass.h"
#include "graphics/vulkan/VulkanRenderer.h"
#include "graphics/vulkan/VulkanPipeline.h"
#include "graphics/vulkan/VulkanBuffer.h"

class VulkanState
{
public:
	static std::shared_ptr<VulkanInstance> Instance;
	static std::shared_ptr<VulkanDevice> Device;
	static std::shared_ptr<VulkanRenderer> Renderer;

public:
	static void Init();
	static void WaitIdle();
};

