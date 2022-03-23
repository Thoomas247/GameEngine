#include "precompiled.h"
#include "VulkanPipeline.h"


/* -- PUBLIC -- */

VulkanPipeline::VulkanPipeline(VulkanPipeline&& oldPipeline) noexcept
{
	m_Instance = oldPipeline.m_Instance;
	m_Device = oldPipeline.m_Device;

	Pipeline = oldPipeline.Pipeline;
	m_PipelineLayout = oldPipeline.m_PipelineLayout;
	m_DescriptorSetLayout = oldPipeline.m_DescriptorSetLayout;
	m_ShaderModules = oldPipeline.m_ShaderModules;
	m_ShaderStageCreateInfo = oldPipeline.m_ShaderStageCreateInfo;

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
		vkDestroyDescriptorSetLayout(m_Device->LogicalDevice, m_DescriptorSetLayout, nullptr);
	}

	Pipeline = VK_NULL_HANDLE;
}

void VulkanPipeline::Init(const std::string& glslPath)
{
	m_Instance = VulkanState::Instance;
	m_Device = VulkanState::Device;

	std::vector<CachedShader> spirvFiles = std::vector<CachedShader>();

	getSpirvFiles(glslPath, spirvFiles);
	compileFromSpirvAndReflect(spirvFiles);
	createPipeline();
}


/* -- PRIVATE -- */

void VulkanPipeline::getSpirvFiles(const std::string& glslPath, std::vector<CachedShader>& spirvFiles)
{
	// notes:
	// [x] shader gets compiled from glsl to spirv
	// [x] spirv is cached
	// [] reflect on shader using spirv-cross to get uniforms
	// [x] on next load, check if glsl file has changed (eg using hash), if changed, re-compile into spirv
	// [] on shader saved, save glsl path and uniform values
	// [] on shader load, compile shader and set saved uniforms

	// load the glsl file
	std::string shaderString = ShaderUtil::LoadGlslFileContents(glslPath);

	// create the directory if it does not exist
	std::string shaderPath = ProjectManager::GetCachePath() + "shaders/";
	std::filesystem::create_directories(shaderPath);

	// simple hashing to detect changes
	std::hash<std::string> stringHash;
	size_t hash = stringHash(shaderString);

	// check if shader has been cached
	std::string cacheFilePath = shaderPath + std::to_string(hash) + ".cache";
	if (std::filesystem::exists(cacheFilePath))
	{
		std::ifstream in(cacheFilePath, std::ios::in | std::ios::binary);
		json j;
		in >> j;
		in.close();
		for (int type = 0; type < (int)ShaderType::NUM_TYPES; type++)
		{
			std::string typeString = ShaderUtil::GetTypeString((ShaderType)type);

			if (!j[typeString].is_null())
			{
				spirvFiles.push_back(CachedShader((ShaderType)type, j[typeString]));
			}
		}
	}
	else
	{
		shaderPath += Util::GetFileName(glslPath);
		json j;

		// split shaders and compile to spirv
		for (int i = 0; i < (int)ShaderType::NUM_TYPES; i++)
		{
			ShaderType type = (ShaderType)i;

			std::string shaderCode = ShaderUtil::SplitShaderStage(shaderString, type);
			if (!shaderCode.empty())
			{
				std::vector<uint32_t> shaderSpirv = ShaderUtil::ConvertToSpirv(shaderCode, type);

				// save to file
				std::string spirvPath = shaderPath + ".spv" + ShaderUtil::GetTypeExtension(type);

				std::ofstream spirvOut(spirvPath, std::ios::out | std::ios::binary);
				spirvOut.write((char*)shaderSpirv.data(), shaderSpirv.size() * sizeof(uint32_t));
				spirvOut.flush();
				spirvOut.close();

				spirvFiles.push_back(CachedShader(type, spirvPath));
				j[ShaderUtil::GetTypeString(type)] = spirvPath;
			}
		}
		std::ofstream cacheFileOut(cacheFilePath, std::ios::out | std::ios::binary);
		cacheFileOut << j;
		cacheFileOut.close();
	}
}

void VulkanPipeline::compileFromSpirvAndReflect(const std::vector<CachedShader>& spirvFiles)
{
	m_ShaderStageCreateInfo.resize(spirvFiles.size());
	m_ShaderModules.resize(spirvFiles.size());
	for (int i = 0; i < spirvFiles.size(); i++)
	{
		// load the shader file from disk
		const CachedShader& spirvShader = spirvFiles.at(i);
		std::vector<uint32_t> spirv;

		std::ifstream fileStream(spirvShader.Path, std::ios::in | std::ios::binary);

		if (!fileStream.is_open())
		{
			LOG_ERROR("VULKAN_PIPELINE::Failed to open spirv file!");
		}

		size_t fileSize = std::filesystem::file_size(spirvShader.Path);
		spirv.resize(fileSize / sizeof(uint32_t));

		fileStream.read((char*)spirv.data(), fileSize);
		fileStream.close();

		// compile shader from spirv
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.codeSize = spirv.size() * sizeof(uint32_t);
		createInfo.pCode = spirv.data();

		if (vkCreateShaderModule(m_Device->LogicalDevice, &createInfo, nullptr, &m_ShaderModules[i]) != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_PIPELINE::Failed to create shader module!");
		}

		// tell vulkan how to use this shader
		m_ShaderStageCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_ShaderStageCreateInfo[i].pNext = nullptr;
		m_ShaderStageCreateInfo[i].stage = ShaderUtil::GetVulkanType(spirvShader.Type);
		m_ShaderStageCreateInfo[i].module = m_ShaderModules[i];
		m_ShaderStageCreateInfo[i].pName = "main";

		// reflect on the shader to retrieve its uniforms, push constants, etc...
		spirv_cross::Compiler compiler = spirv_cross::Compiler(spirv);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (auto& ubo : resources.uniform_buffers)
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding{};
			uboLayoutBinding.binding = compiler.get_decoration(ubo.id, spv::DecorationBinding);
			uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uboLayoutBinding.descriptorCount = 1;	// TODO: Find way to get descriptor count using spirv_cross
			uboLayoutBinding.stageFlags = ShaderUtil::GetVulkanType(spirvShader.Type);

			m_LayoutBindings.push_back(uboLayoutBinding);
		}

		// TODO: Get push constants here too
	}

	// create descriptor set layout now that descriptors have been retrieved
	VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = m_LayoutBindings.size();
	layoutCreateInfo.pBindings = m_LayoutBindings.data();

	if (vkCreateDescriptorSetLayout(m_Device->LogicalDevice, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
		LOG_ERROR("VULKAN_PIPELINE::Failed to create descriptor set layout!");
	}
}

void VulkanPipeline::createPipeline()
{
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	// pipeline layout create info
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};\
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = m_LayoutBindings.size();
	pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
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

	pipelineCreateInfo.stageCount = (uint32_t)m_ShaderStageCreateInfo.size();
	pipelineCreateInfo.pStages = m_ShaderStageCreateInfo.data();
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
