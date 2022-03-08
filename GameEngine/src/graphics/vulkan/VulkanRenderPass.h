#pragma once

class VulkanRenderPass
{
public:
	VkRenderPass RenderPass;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

public:
	VulkanRenderPass(VkFormat colorFormat);
	~VulkanRenderPass();
};

