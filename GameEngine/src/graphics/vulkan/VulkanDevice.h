#pragma once

class VulkanDevice
{
public:
	VkPhysicalDevice PhysicalDevice;
	VkDevice LogicalDevice;

	VkPhysicalDeviceProperties Properties;
	VkPhysicalDeviceFeatures Features;
	VkPhysicalDeviceFeatures EnabledFeatures;
	VkPhysicalDeviceMemoryProperties MemoryProperties;

	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	QueueIndices QueueFamilyIndices;

	VkQueue GraphicsQueue;
	VkQueue ComputeQueue;
	VkQueue TransferQueue;

	std::vector<std::string> SupportedExtensions;
	VkCommandPool CommandPool = VK_NULL_HANDLE;

private:
	bool m_EnableDebugMarkers;

public:
	VulkanDevice(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions);
	~VulkanDevice();

	void Cleanup();

	uint32_t GetMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = nullptr);
	uint32_t GetQueueFamilyIndex(VkQueueFlagBits queueFlags);

	VkResult CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data = nullptr);
	void  CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	
	VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin = false);
	VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel level, bool begin = false);

	bool ExtensionSupported(const std::string& extension);

	VkFormat GetSupportedDepthFormat(bool checkSamplingSupport);

private:
	void createLogicalDevice(std::vector<const char*> enabledExtensions, void* pNextChain, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	void createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
};

