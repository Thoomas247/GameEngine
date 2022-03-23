#include "precompiled.h"
#include "VulkanBuffer.h"


/* -- PUBLIC -- */

VulkanBuffer::VulkanBuffer(VulkanBuffer&& oldBuffer) noexcept
{
	m_Instance = oldBuffer.m_Instance;
	m_Device = oldBuffer.m_Device;

	Buffer = oldBuffer.Buffer;
	BufferMemory = oldBuffer.BufferMemory;

	oldBuffer.Buffer = VK_NULL_HANDLE;
	oldBuffer.BufferMemory = VK_NULL_HANDLE;
}

VulkanBuffer::~VulkanBuffer()
{
	if (Buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_Device->LogicalDevice, Buffer, nullptr);
		vkFreeMemory(m_Device->LogicalDevice, BufferMemory, nullptr);
	}

	Buffer = VK_NULL_HANDLE;
}

void VulkanBuffer::Init(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data)
{
	if (Buffer != VK_NULL_HANDLE)
	{
		LOG_ERROR("VULKAN_BUFFER::Buffer is being initialized more than once!");
	}

	m_Size = size;
	m_HostCoherent = false;

	m_Instance = VulkanState::Instance;
	m_Device = VulkanState::Device;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkResult result = m_Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		size, &stagingBuffer, &stagingBufferMemory, data);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_BUFFER::Failed to create staging buffer!");
	}

	result = m_Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageFlag, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		size, &Buffer, &BufferMemory);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_BUFFER::Failed to create buffer!");
	}

	m_Device->CopyBuffer(stagingBuffer, Buffer, size);

	vkDestroyBuffer(m_Device->LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_Device->LogicalDevice, stagingBufferMemory, nullptr);
}

void VulkanBuffer::InitHostCoherent(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data)
{
	if (Buffer != VK_NULL_HANDLE)
	{
		LOG_ERROR("VULKAN_BUFFER::Buffer is being initialized more than once!");
	}

	m_Size = size;
	m_HostCoherent = true;

	m_Instance = VulkanState::Instance;
	m_Device = VulkanState::Device;

	VkResult result = m_Device->CreateBuffer(usageFlag, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		size, &Buffer, &BufferMemory, data);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_BUFFER::Failed to create buffer!");
	}
}

void VulkanBuffer::SetData(void* data)
{
	if (!m_HostCoherent)
	{
		LOG_WARN("VULKAN_BUFFER::Changing buffer data when buffer was not initialized as \"host coherent\". This is very slow, please use VulkanBuffer::InitHostCoherent instead.");
	}

	vkMapMemory(m_Device->LogicalDevice, BufferMemory, 0, m_Size, 0, &data);
	memcpy(data, data, m_Size);
	vkUnmapMemory(m_Device->LogicalDevice, BufferMemory);
}
