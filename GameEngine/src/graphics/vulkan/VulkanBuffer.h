#pragma once

class VulkanBuffer
{
public:
	VkBuffer Buffer;
	VkDeviceMemory BufferMemory;

public:
	VulkanBuffer(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data = nullptr);

	void Cleanup();
};

