#pragma once

constexpr auto DEFAULT_SWAPCHAIN_WIDTH = 800;
constexpr auto DEFAULT_SWAPCHAIN_HEIGHT = 600;
constexpr auto DEFAULT_SWAPCHAIN_VSYNC = false;

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

private:
	VkRenderPass m_RenderPass;

public:
	VulkanSwapChain();
	void Cleanup();

	void Create(VkRenderPass renderPass, uint32_t fallbackWidth = DEFAULT_SWAPCHAIN_WIDTH, uint32_t fallbackHeight = DEFAULT_SWAPCHAIN_HEIGHT, bool vsync = DEFAULT_SWAPCHAIN_VSYNC);
	void Recreate(uint32_t fallbackWidth, uint32_t fallbackHeight, bool vsync = false);

private:
	void setQueues();
	void setColorFormat();
	void createSwapChain(uint32_t fallbackWidth, uint32_t fallbackHeight, bool vsync);
};
