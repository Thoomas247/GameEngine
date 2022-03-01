#pragma once

class VulkanSwapChain
{
public:
	VkFormat ColorFormat;
	VkColorSpaceKHR ColorSpace;
	VkSwapchainKHR SwapChain = VK_NULL_HANDLE;
	uint32_t ImageCount;
	std::vector<VkFramebuffer> FrameBuffers;
	std::vector<VkImage> Images;
	std::vector<SwapChainBuffer> Buffers;
	uint32_t GraphicsQueueNodeIndex = UINT32_MAX;
	uint32_t PresentQueueNodeIndex = UINT32_MAX;
	VkExtent2D Extent;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

public:
	VulkanSwapChain(const std::shared_ptr<VulkanInstance>& instance, const std::shared_ptr<VulkanDevice>& device);
	~VulkanSwapChain();

	void Create(VkRenderPass renderPass, uint32_t* width, uint32_t* height, bool vsync = false);
	void Cleanup();
};
