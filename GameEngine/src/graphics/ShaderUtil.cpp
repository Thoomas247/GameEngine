#include "precompiled.h"
#include "ShaderUtil.h"


/* -- PUBLIC -- */

std::string ShaderUtil::LoadGlslFileContents(const std::string& absolutePath)
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
		LOG_ERROR("SHADER_UTIL::File at path " + absolutePath + " not successfully read.");
	}

	return fileContents;
}

std::string ShaderUtil::SplitShaderStage(const std::string& shaderString, const ShaderType& shaderType)
{
	std::string startTag = "#start " + GetTypeString(shaderType);
	std::string endTag = "#end " + GetTypeString(shaderType);

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
		LOG_ERROR("SHADER_UTIL::Shader end tag (" + endTag + ") not found!");
	}

	return shaderString.substr(startPos, endPos - startPos).c_str();
}

std::vector<uint32_t> ShaderUtil::ConvertToSpirv(const std::string& shaderString, const ShaderType& shaderType)
{
	shaderc::Compiler compiler = shaderc::Compiler();
	shaderc::CompileOptions compileOptions;
	compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, 0);
	compileOptions.SetAutoBindUniforms(true);
	compileOptions.SetAutoMapLocations(true);
	compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderString, GetShadercType(shaderType), "Shader");
	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		LOG_ERROR("SHADER::Shader compilation failed! (" + result.GetErrorMessage() + ")");
	}

	return std::vector<uint32_t>(result.cbegin(), result.cend());
}

VkShaderStageFlagBits ShaderUtil::GetVulkanType(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::vertex:
		return VK_SHADER_STAGE_VERTEX_BIT;

	case ShaderType::fragment:
		return VK_SHADER_STAGE_FRAGMENT_BIT;

	default:
		LOG_ERROR("SHADER_UTIL::Shader type is invalid!");
	}
}

VkDescriptorType ShaderUtil::GetVulkanType(const DescriptorType& type)
{
	switch (type)
	{
	case DescriptorType::uniform_buffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	default:
		LOG_ERROR("SHADER_UTIL::Descriptor type is invalid!");
	}
}

shaderc_shader_kind ShaderUtil::GetShadercType(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::vertex:
		return shaderc_vertex_shader;

	case ShaderType::fragment:
		return shaderc_fragment_shader;

	default:
		LOG_ERROR("SHADER_UTIL::Shader type is invalid!");
	}
}

std::string ShaderUtil::GetTypeString(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::vertex:
		return "vertex";

	case ShaderType::fragment:
		return "fragment";

	default:
		LOG_ERROR("SHADER_UTIL::Shader type is invalid!");
	}
}

std::string ShaderUtil::GetTypeString(const DescriptorType& type)
{
	switch (type)
	{
	case DescriptorType::uniform_buffer:
		return "uniform_buffer";

	default:
		LOG_ERROR("SHADER_UTIL::Descriptor type is invalid!");
	}
}

std::string ShaderUtil::GetTypeExtension(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::vertex:
		return ".vert";

	case ShaderType::fragment:
		return ".frag";

	default:
		LOG_ERROR("SHADER_UTIL::Shader type is invalid!");
	}
}