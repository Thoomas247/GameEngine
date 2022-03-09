#pragma once

class VulkanBuffer
{
public:
	VkBuffer Buffer = VK_NULL_HANDLE;
	VkDeviceMemory BufferMemory = VK_NULL_HANDLE;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

public:
	VulkanBuffer() = default;
	VulkanBuffer(VulkanBuffer&& oldBuffer) noexcept;
	~VulkanBuffer();

	void Init(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data = nullptr);
};

