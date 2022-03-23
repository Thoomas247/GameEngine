#pragma once

class VulkanBuffer
{
public:
	VkBuffer Buffer = VK_NULL_HANDLE;
	VkDeviceMemory BufferMemory = VK_NULL_HANDLE;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

	VkDeviceSize m_Size;
	bool m_HostCoherent = false;

public:
	VulkanBuffer() = default;
	VulkanBuffer(VulkanBuffer&& oldBuffer) noexcept;
	~VulkanBuffer();

	/// <summary>
	/// Create vulkan buffer whose data will not be changed until deletion.
	/// </summary>
	/// <param name="usageFlag"></param>
	/// <param name="size"></param>
	/// <param name="data"></param>
	void Init(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data = nullptr);
	/// <summary>
	/// Create vulkan buffer whose data can be changed.
	/// </summary>
	/// <param name="usageFlag"></param>
	/// <param name="size"></param>
	/// <param name="data"></param>
	void InitHostCoherent(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data = nullptr);
	/// <summary>
	/// Sets the data contained in the buffer so it can be used by the GPU.
	/// Will create a warning if the buffer was not initialized as "host coherent".
	/// </summary>
	/// <param name="data"></param>
	void SetData(void* data);
};

