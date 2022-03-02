#include "precompiled.h"
#include "MeshInfo.h"


/* -- PUBLIC -- */

MeshInfo::MeshInfo(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	// vertex buffer
	VkResult result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		sizeof(vertices[0]) * vertices.size(), &VertexBuffer, &VertexBufferMemory, (void*)vertices.data());

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create vertex buffer!");
	}

	// index buffer
	result = VulkanState::Device->CreateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		sizeof(indices[0]) * indices.size(), &IndexBuffer, &IndexBufferMemory, (void*)indices.data());
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("MESH_INFO::Failed to create index buffer!");
	}

	NumIndices = indices.size();

	// vertex description
	VertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputCreateInfo.pNext = nullptr;
	VertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	VertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	VertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
	VertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// input assembly
	InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyInfo.pNext = nullptr;
	InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// rasterizer
	RasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationCreateInfo.pNext = nullptr;
	RasterizationCreateInfo.depthClampEnable = VK_FALSE;
	RasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;

	RasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;	// points, lines, or filled
	RasterizationCreateInfo.lineWidth = 1.0f;

	RasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE;
	RasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

	RasterizationCreateInfo.depthBiasEnable = VK_FALSE;
	RasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
	RasterizationCreateInfo.depthBiasClamp = 0.0f;
	RasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;
}

MeshInfo::~MeshInfo()
{
	if (VertexBuffer != VK_NULL_HANDLE || IndexBuffer != VK_NULL_HANDLE)
	{
		Cleanup();
	}
}

void MeshInfo::Cleanup()
{
	vkDestroyBuffer(VulkanState::Device->LogicalDevice, VertexBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, VertexBufferMemory, nullptr);

	vkDestroyBuffer(VulkanState::Device->LogicalDevice, IndexBuffer, nullptr);
	vkFreeMemory(VulkanState::Device->LogicalDevice, IndexBufferMemory, nullptr);

	VertexBuffer = VK_NULL_HANDLE;
	IndexBuffer = VK_NULL_HANDLE;
}
