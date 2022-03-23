#pragma once

class VulkanRenderer
{
public:
	std::unique_ptr<VulkanSwapChain> SwapChain;
	std::unique_ptr<VulkanRenderPass> RenderPass;	// renderpass must be initialized after swapchain

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

	bool m_ResizeRequested;

	uint32_t m_CurrentFrameIndex;
	uint32_t m_CurrentSwapChainImageIndex;

	std::vector<VkCommandBuffer> m_CommandBuffers;
	std::vector<VkSemaphore> m_RenderCompleteSemaphores;
	std::vector<VkSemaphore> m_PresentCompleteSemaphores;
	std::vector<VkFence> m_RenderFences;

	VkClearValue m_ClearValue[2];

public:
	VulkanRenderer();
	~VulkanRenderer();

	void StartRendering();
	void Submit(VkPipeline pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer, const uint32_t& indexCount);
	void FinishRendering();

	void RequestResize();

private:
	void initCommandBuffers();
	void initSemaphoresAndFences();
	void setClearValue(float r, float g, float b, float a, float depth);

	void recreateSwapChain();

	void prepareFrame();
	void beginCommandBuffer();
	void beginRenderPass();

	void endRenderPass();
	void endCommandBuffer();
	void queueSubmit();
	void queuePresent();
};

