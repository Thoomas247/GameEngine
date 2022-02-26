#include "precompiled.h"
#include "ShaderUtil.h"

/* -- PUBLIC -- */

int ShaderUtil::GetOpenglType(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::vertex:
		return GL_VERTEX_SHADER;

	case ShaderType::fragment:
		return GL_FRAGMENT_SHADER;

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