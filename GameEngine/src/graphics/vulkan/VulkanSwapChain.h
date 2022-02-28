#pragma once

class VulkanSwapChain
{
private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

public:
	VkFormat ColorFormat;
	VkColorSpaceKHR ColorSpace;
	VkSwapchainKHR SwapChain = VK_NULL_HANDLE;
	uint32_t ImageCount;
	std::vector<VkImage> Images;
	std::vector<SwapChainBuffer> Buffers;
	uint32_t GraphicsQueueNodeIndex = UINT32_MAX;
	uint32_t PresentQueueNodeIndex = UINT32_MAX;

public:
	VulkanSwapChain(const std::shared_ptr<VulkanInstance>& instance, const std::shared_ptr<VulkanDevice>& device);
	~VulkanSwapChain();

	void Create(uint32_t* width, uint32_t* height, bool vsync = false);
	void Cleanup();

	VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
	VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);
};

