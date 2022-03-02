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

	VkViewport Viewport;
	VkRect2D Scissor;

public:
	VulkanSwapChain();
	~VulkanSwapChain();

	void Create(uint32_t fallbackWidth, uint32_t fallbackHeight, bool vsync = false);
	void Cleanup();
};
