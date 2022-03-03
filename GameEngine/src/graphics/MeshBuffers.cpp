#include "precompiled.h"
#include "MeshBuffers.h"


/* -- PUBLIC -- */

MeshBuffers::MeshBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	// vertex buffer
	createVertexBuffer(vertices);

	// index buffer
	createIndexBuffer(indices);

	NumIndices = indices.size();
}

void MeshBuffers::Cleanup()
{
	vkDestroyBuffer(VulkanState::Device->LogicalDevice, VertexBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, VertexBufferMemory, nullptr);

	vkDestroyBuffer(VulkanState::Device->LogicalDevice, IndexBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, IndexBufferMemory, nullptr);

	VertexBuffer = VK_NULL_HANDLE;
	IndexBuffer = VK_NULL_HANDLE;
}


/* -- PUBLIC -- */

void MeshBuffers::createVertexBuffer(const std::vector<Vertex>& vertices)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkDeviceSize size = sizeof(vertices[0]) * vertices.size();

	VkResult result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		size, &stagingBuffer, &stagingBufferMemory, (void*)vertices.data());

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create vertex staging buffer!");
	}

	result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		size, &VertexBuffer, &VertexBufferMemory);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create vertex buffer!");
	}

	VulkanState::Device->CopyBuffer(stagingBuffer, VertexBuffer, size);

	vkDestroyBuffer(VulkanState::Device->LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, stagingBufferMemory, nullptr);
}

void MeshBuffers::createIndexBuffer(const std::vector<uint32_t>& indices)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkDeviceSize size = sizeof(indices[0]) * indices.size();

	VkResult result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		size, &stagingBuffer, &stagingBufferMemory, (void*)indices.data());

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create index staging buffer!");
	}

	result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		size, &IndexBuffer, &IndexBufferMemory);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create index buffer!");
	}

	VulkanState::Device->CopyBuffer(stagingBuffer, IndexBuffer, size);

	vkDestroyBuffer(VulkanState::Device->LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, stagingBufferMemory, nullptr);
}
