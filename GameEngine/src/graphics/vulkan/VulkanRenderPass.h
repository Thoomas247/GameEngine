#pragma once

class VulkanRenderPass
{
public:
	VkRenderPass RenderPass;

public:
	VulkanRenderPass();
	~VulkanRenderPass();

	void Cleanup();
};

