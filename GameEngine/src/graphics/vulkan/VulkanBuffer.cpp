#include "precompiled.h"
#include "VulkanBuffer.h"


/* -- PUBLIC -- */

VulkanBuffer::VulkanBuffer(VkBufferUsageFlagBits usageFlag, VkDeviceSize size, void* data)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkResult result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		size, &stagingBuffer, &stagingBufferMemory, data);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create vertex staging buffer!");
	}

	result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageFlag, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		size, &Buffer, &BufferMemory);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create vertex buffer!");
	}

	VulkanState::Device->CopyBuffer(stagingBuffer, Buffer, size);

	vkDestroyBuffer(VulkanState::Device->LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, stagingBufferMemory, nullptr);

	// add to VukanState so it can be cleaned up
	VulkanState::Buffers.push_back(this);
}

void VulkanBuffer::Cleanup()
{
	vkDestroyBuffer(VulkanState::Device->LogicalDevice, Buffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, BufferMemory, nullptr);

	Buffer = VK_NULL_HANDLE;
}
