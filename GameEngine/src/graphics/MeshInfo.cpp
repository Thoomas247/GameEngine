#include "precompiled.h"
#include "MeshInfo.h"

/* -- PUBLIC -- */

MeshInfo::MeshInfo()
{
	// TODO: set these based on passed vertices and indices

	// vertex input
	VertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputCreateInfo.pNext = nullptr;
	VertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	VertexInputCreateInfo.vertexBindingDescriptionCount = 0;

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
