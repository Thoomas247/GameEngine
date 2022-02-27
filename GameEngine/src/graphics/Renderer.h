#pragma once

/*
* Implementation base on: https://vulkan-tutorial.com/Introduction
*/

struct QueueFamilyIndices {
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete() {
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

class Renderer
{
private:
	static glm::mat4 s_ViewMatrix;
	static glm::mat4 s_ProjectionMatrix;

	static VkInstance s_VkInstance;
	static VkPhysicalDevice s_PhysicalDevice;
	static VkDevice s_LogicalDevice;

	static VkQueue s_GraphicsQueue;
	static VkQueue s_PresentQueue;

	static VkSurfaceKHR s_Surface;

	static VkSwapchainKHR s_SwapChain;
	static std::vector<VkImage> s_SwapChainImages;
	static std::vector<VkImageView> s_SwapChainImageViews;
	static VkFormat s_SwapChainImageFormat;
	static VkExtent2D s_SwapChainExtent;

	static VkRenderPass s_RenderPass;
	static VkPipelineLayout s_PipelineLayout;
	static VkPipeline s_GraphicsPipeline;

	static std::vector<VkFramebuffer> s_SwapChainFramebuffers;

	static VkCommandPool s_CommandPool;
	static std::vector<VkCommandBuffer> s_CommandBuffers;

	static std::vector<VkSemaphore> s_ImageAvailableSemaphores;
	static std::vector<VkSemaphore> s_RenderFinishedSemaphores;
	static std::vector<VkFence> s_InFlightFences;

	static bool s_FrameBufferResized;

public:
	static void Init();
	static void Draw();
	static void CleanUp();
	static void RequestResize() { s_FrameBufferResized = true; }

	static void SetViewMatrix(const glm::mat4& viewMatrix) { s_ViewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const glm::mat4& projectionMatrix) { s_ProjectionMatrix = projectionMatrix; }

private:
	static void createInstance();
	static void createSwapChain();
	static void getAvailableExtensions();
	static void createSurface();
	static void pickPhysicalDevice();
	static bool isDeviceSuitable(VkPhysicalDevice device);
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	static void createLogicalDevice();
	static void createImageViews();
	static void createRenderPass();
	static void createGraphicsPipeline();
	static VkShaderModule createShaderModule(const std::vector<char>& code);
	static void createFramebuffers();
	static void createCommandPool();
	static void createCommandBuffers();
	static void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	static void createSyncObjects();
	static void cleanupSwapChain();
	static void recreateSwapChain();
};