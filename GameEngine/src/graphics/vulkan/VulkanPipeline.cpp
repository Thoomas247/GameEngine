#include "precompiled.h"
#include "VulkanPipeline.h"


/* -- PUBLIC -- */

VulkanPipeline::VulkanPipeline(const std::string& glslPath)
{
	std::vector<CachedShader> spirvFiles = std::vector<CachedShader>();

	getSpirvFiles(glslPath, spirvFiles);
	compileFromSpirv(spirvFiles);
	createPipeline();

	// add to VukanState so it can be cleaned up
	// TODO: add vulkan's pointers directly and remove cleanup from here
	VulkanState::Pipelines.push_back(this);
}

void VulkanPipeline::Cleanup()
{
	for (int i = 0; i < m_ShaderModules.size(); i++)
	{
		vkDestroyShaderModule(VulkanState::Device->LogicalDevice, m_ShaderModules[i], nullptr);
	}
	vkDestroyPipeline(VulkanState::Device->LogicalDevice, Pipeline, nullptr);
	vkDestroyPipelineLayout(VulkanState::Device->LogicalDevice, m_PipelineLayout, nullptr);
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
	std::string shaderString = loadGlslFileContents(glslPath);

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

			std::string shaderCode = splitShader(shaderString, ShaderUtil::GetTypeString(type));
			if (!shaderCode.empty())
			{
				std::vector<uint32_t> shaderSpirv = compileToSpirv(shaderCode, ShaderUtil::GetShadercType(type));

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

std::string VulkanPipeline::loadGlslFileContents(const std::string& absolutePath)
{
	std::string fileContents;
	std::ifstream fileStream(absolutePath);

	if (fileStream.is_open())
	{
		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		fileStream.close();

		fileContents = stringStream.str();
	}
	else
	{
		LOG_ERROR("SHADER::File at path " + absolutePath + " not successfully read.");
	}

	return fileContents;
}

std::string VulkanPipeline::splitShader(const std::string& shaderString, const std::string& shaderType)
{
	std::string startTag = "#start " + shaderType;
	std::string endTag = "#end " + shaderType;

	std::smatch match;

	// shader start
	std::regex startRegex = std::regex("(" + startTag + ")");
	std::regex_search(shaderString, match, startRegex);
	size_t startPos = match.position(0) + startTag.size() + 1;

	if (match.size() == 0)
	{
		return std::string();
	}

	// shader end
	std::regex endRegex = std::regex("(" + endTag + ")");
	std::regex_search(shaderString, match, endRegex);
	size_t endPos = match.position(0) - 1;

	if (match.size() == 0)
	{
		LOG_ERROR("SHADER::Shader end tag (" + endTag + ") not found!");
	}

	return shaderString.substr(startPos, endPos - startPos).c_str();
}

std::vector<uint32_t> VulkanPipeline::compileToSpirv(const std::string& shaderString, const shaderc_shader_kind& type)
{
	shaderc::Compiler compiler = shaderc::Compiler();
	shaderc::CompileOptions compileOptions;
	compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, 0);
	compileOptions.SetAutoBindUniforms(true);
	compileOptions.SetAutoMapLocations(true);
	compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderString, type, "Shader");
	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		LOG_ERROR("SHADER::Shader compilation failed! (" + result.GetErrorMessage() + ")");
	}

	return std::vector<uint32_t>(result.cbegin(), result.cend());
}

void VulkanPipeline::compileFromSpirv(const std::vector<CachedShader>& spirvFiles)
{
	m_ShaderStageCreateInfo.resize(spirvFiles.size());
	m_ShaderModules.resize(spirvFiles.size());
	for (int i = 0; i < spirvFiles.size(); i++)
	{
		const CachedShader& spirvShader = spirvFiles.at(i);
		std::vector<uint32_t> spirv = loadSpirvFileContents(spirvShader.Path);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;

		createInfo.codeSize = spirv.size() * sizeof(uint32_t);
		createInfo.pCode = spirv.data();

		if (vkCreateShaderModule(VulkanState::Device->LogicalDevice, &createInfo, nullptr, &m_ShaderModules[i]) != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_PIPELINE::Failed to create shader module!");
		}

		m_ShaderStageCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_ShaderStageCreateInfo[i].pNext = nullptr;
		m_ShaderStageCreateInfo[i].stage = ShaderUtil::GetVulkanType(spirvShader.Type);
		m_ShaderStageCreateInfo[i].module = m_ShaderModules[i];
		m_ShaderStageCreateInfo[i].pName = "main";
	}

	// the shaders determine the number of inputs, outputs and push constants so this goes here
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.pNext = nullptr;

	if (vkCreatePipelineLayout(VulkanState::Device->LogicalDevice, &createInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_PIPELINE::Failed to create pipeline layout!");
	}
}

std::vector<uint32_t> VulkanPipeline::loadSpirvFileContents(const std::string& absolutePath)
{
	std::vector<uint32_t> data;
	std::ifstream fileStream(absolutePath, std::ios::in | std::ios::binary);

	if (!fileStream.is_open())
	{
		LOG_ERROR("VULKAN_PIPELINE::Failed to open spirv file!");
	}

	size_t fileSize = std::filesystem::file_size(absolutePath);
	data.resize(fileSize / sizeof(uint32_t));

	fileStream.read((char*)data.data(), fileSize);
	fileStream.close();

	return data;
}

void VulkanPipeline::reflect(const std::vector<uint32_t>& shaderWords)
{
	spirv_cross::Compiler compiler = spirv_cross::Compiler(shaderWords);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	for (const auto& resource : resources.uniform_buffers)
	{
		// get buffer name and type
		std::string bufferName = resource.name;
		const auto& bufferType = compiler.get_type(resource.base_type_id);

		std::vector<Uniform> uniformData;

		// loop through uniforms
		const auto& uniforms = bufferType.member_types;
		for (int i = 0; i < uniforms.size(); i++)
		{
			//std::string uniformName = compiler.get_member_name(resource.base_type_id, i);
			//const auto& uniformType = compiler.get_type(bufferType.member_types[i]);
		}
	}
}

void VulkanPipeline::createPipeline()
{
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	// TODO: set all of these settings (eg cull mode and polygon mode) based on variables in meshInfo

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

	VkPipelineViewportStateCreateInfo viewportCreateInfo{};
	viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportCreateInfo.pNext = nullptr;
	viewportCreateInfo.viewportCount = 1;
	viewportCreateInfo.pViewports = &VulkanState::Renderer->SwapChain->Viewport;
	viewportCreateInfo.scissorCount = 1;
	viewportCreateInfo.pScissors = &VulkanState::Renderer->SwapChain->Scissor;

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
	pipelineCreateInfo.layout = m_PipelineLayout;
	pipelineCreateInfo.renderPass = VulkanState::Renderer->RenderPass->RenderPass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(VulkanState::Device->LogicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &Pipeline) != VK_SUCCESS)
	{
		LOG_ERROR("RENDER_PIPELINE::Failed to create graphics pipeline!");
	}
}
