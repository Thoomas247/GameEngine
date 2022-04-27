#include "precompiled.h"
#include "VulkanPipeline.h"


/* -- PUBLIC -- */

VulkanPipeline::VulkanPipeline(VulkanPipeline&& oldPipeline) noexcept
{
	m_Instance = oldPipeline.m_Instance;
	m_Device = oldPipeline.m_Device;

	Pipeline = oldPipeline.Pipeline;
	m_PipelineLayout = oldPipeline.m_PipelineLayout;
	
	m_ShaderModules = oldPipeline.m_ShaderModules;

	oldPipeline.Pipeline = VK_NULL_HANDLE;
}

VulkanPipeline::~VulkanPipeline()
{
	if (Pipeline != VK_NULL_HANDLE)
	{
		for (int i = 0; i < m_ShaderModules.size(); i++)
		{
			vkDestroyShaderModule(m_Device->LogicalDevice, m_ShaderModules[i], nullptr);
		}
		vkDestroyPipeline(m_Device->LogicalDevice, Pipeline, nullptr);
		vkDestroyPipelineLayout(m_Device->LogicalDevice, m_PipelineLayout, nullptr);
	}

	Pipeline = VK_NULL_HANDLE;
}

void VulkanPipeline::Init(const std::vector<SpirvCodeInfo>& spirvCode, VkDescriptorSetLayout descriptorSetLayout, const uint32_t& setLayoutCount)
{
	m_Instance = VulkanState::Instance;
	m_Device = VulkanState::Device;

	compileShaderStages(spirvCode);
	createPipeline(descriptorSetLayout, setLayoutCount);
}


/* -- PRIVATE -- */

void VulkanPipeline::compileShaderStages(const std::vector<SpirvCodeInfo>& spirvCode)
{
	uint32_t numShaders = spirvCode.size();

	m_ShaderModules.resize(numShaders);

	for (int i = 0; i < numShaders; i++)
	{
		// compile shader from spirv
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.codeSize = spirvCode[i].Code.size() * sizeof(uint32_t);
		createInfo.pCode = spirvCode[i].Code.data();

		if (vkCreateShaderModule(m_Device->LogicalDevice, &createInfo, nullptr, &m_ShaderModules[i]) != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_PIPELINE::Failed to create shader module!");
		}

		// tell vulkan how to use this shader module
		VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
		shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageCreateInfo.pNext = nullptr;
		shaderStageCreateInfo.stage = ShaderUtil::GetVulkanType(spirvCode[i].Type);
		shaderStageCreateInfo.module = m_ShaderModules[i];
		shaderStageCreateInfo.pName = "main";

		m_ShaderStageCreateInfos.push_back(shaderStageCreateInfo);
	}
}

void VulkanPipeline::createPipeline(VkDescriptorSetLayout descriptorSetLayout, const uint32_t& setLayoutCount)
{
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	// pipeline layout create info
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pNext = nullptr;

	if (vkCreatePipelineLayout(m_Device->LogicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_PIPELINE::Failed to create pipeline layout!");
	}

	// vertex input create info
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.pNext = nullptr;
	vertexInputCreateInfo.flags = 0;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();//Vertex::NUM_VERTEX_ATTRIBS;
	vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo{};
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCreateInfo.pNext = nullptr;
	rasterizationCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationCreateInfo.flags = 0;

	rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;	// points, lines, or filled
	rasterizationCreateInfo.lineWidth = 1.0f;

	rasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE;
	rasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

	rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationCreateInfo.depthBiasClamp = 0.0f;
	rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;

	// TODO: set proper sampling settings here
	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo{};
	multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleCreateInfo.pNext = nullptr;
	multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleCreateInfo.pSampleMask = nullptr;
	multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleCreateInfo.alphaToOneEnable = VK_FALSE;


	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
		//VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkPipelineViewportStateCreateInfo viewportCreateInfo{};
	viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportCreateInfo.pNext = nullptr;
	viewportCreateInfo.viewportCount = 1;
	viewportCreateInfo.pViewports = nullptr;
	viewportCreateInfo.scissorCount = 1;
	viewportCreateInfo.pScissors = nullptr;

	// TODO: set proper blend settings here
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

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
	pipelineCreateInfo.flags = 0;

	pipelineCreateInfo.stageCount = m_ShaderStageCreateInfos.size();
	pipelineCreateInfo.pStages = m_ShaderStageCreateInfos.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineCreateInfo.pViewportState = &viewportCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
	pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
	pipelineCreateInfo.pDynamicState = &dynamicState;
	pipelineCreateInfo.layout = m_PipelineLayout;
	pipelineCreateInfo.renderPass = VulkanState::Renderer->RenderPass->RenderPass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(m_Device->LogicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &Pipeline) != VK_SUCCESS)
	{
		LOG_ERROR("RENDER_PIPELINE::Failed to create graphics pipeline!");
	}
}
