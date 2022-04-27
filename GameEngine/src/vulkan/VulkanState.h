#pragma once

#include "StructsAndConstants.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorPool.h"

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

