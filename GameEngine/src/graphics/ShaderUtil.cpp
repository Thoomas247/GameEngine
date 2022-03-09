#include "precompiled.h"
#include "ShaderUtil.h"


/* -- PUBLIC -- */

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