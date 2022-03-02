#include "precompiled.h"
#include "RenderPipeline.h"


/* -- PUBLIC -- */

RenderPipeline::RenderPipeline(const MaterialShader& shader, const MeshInfo& meshInfo)
{
	// TODO: set proper sampling settings here
	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo{};
	multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleCreateInfo.pNext = nullptr;
	multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleCreateInfo.pSampleMask = nullptr;
	multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportCreateInfo{};
	viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportCreateInfo.pNext = nullptr;
	viewportCreateInfo.viewportCount = 1;
	viewportCreateInfo.pViewports = Renderer::GetViewport();
	viewportCreateInfo.scissorCount = 1;
	viewportCreateInfo.pScissors = Renderer::GetScissor();

	// TODO: set proper blend settings here
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;

	VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo{};
	colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCreateInfo.pNext = nullptr;
	colorBlendCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendCreateInfo.attachmentCount = 1;
	colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
	////////////////////////////////////////

	VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = nullptr;

	pipelineCreateInfo.stageCount = shader.ShaderStageCreateInfo.size();
	pipelineCreateInfo.pStages = shader.ShaderStageCreateInfo.data();
	pipelineCreateInfo.pVertexInputState = &meshInfo.VertexInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &meshInfo.InputAssemblyInfo;
	pipelineCreateInfo.pViewportState = &viewportCreateInfo;
	pipelineCreateInfo.pRasterizationState = &meshInfo.RasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
	pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
	pipelineCreateInfo.layout = shader.PipelineLayout;
	pipelineCreateInfo.renderPass = Renderer::GetRenderPass();
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(Renderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &Pipeline) != VK_SUCCESS)
	{
		LOG_ERROR("RENDER_PIPELINE::Failed to create graphics pipeline!");
	}
}

