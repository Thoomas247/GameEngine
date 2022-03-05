#pragma once

class VulkanRenderPass
{
public:
	VkRenderPass RenderPass;

public:
	VulkanRenderPass(VkFormat colorFormat);

	void Cleanup();
};

