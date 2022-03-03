#pragma once

class VulkanRenderer
{
private:

	static bool s_ResizeRequested;

	static uint32_t s_CurrentFrameIndex;
	static uint32_t s_CurrentSwapChainImageIndex;

	static std::vector<VkCommandBuffer> s_CommandBuffers;
	static std::vector<VkSemaphore> s_RenderCompleteSemaphores;
	static std::vector<VkSemaphore> s_PresentCompleteSemaphores;
	static std::vector<VkFence> s_RenderFences;

	static VkClearValue s_ClearValue[2];

public:
	static void Init();
	static void StartRendering();
	static void Submit(VkPipeline pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer, const uint32_t& indexCount);
	static void FinishRendering();
	static void Cleanup();
	static void RequestResize();

private:
	static void initCommandBuffers();
	static void initSemaphoresAndFences();
	static void setClearValue(float r, float g, float b, float a, float depth);

	static void recreateSwapChain();

	static void prepareFrame();
	static void beginCommandBuffer();
	static void beginRenderPass();

	static void endRenderPass();
	static void endCommandBuffer();
	static void queueSubmit();
	static void queuePresent();
};

