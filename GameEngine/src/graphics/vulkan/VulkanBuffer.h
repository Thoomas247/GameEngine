#pragma once

class VulkanBuffer
{
public:
	VkBuffer Buffer;
	VkDeviceMemory BufferMemory;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

public:
	VulkanBuffer(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data = nullptr);
	~VulkanBuffer();

	void Cleanup();
};

