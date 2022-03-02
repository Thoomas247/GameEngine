#pragma once

enum class ShaderType
{
	vertex, fragment,

	// keep this at the end
	NUM_TYPES
};

class ShaderUtil
{
public:
	static VkShaderStageFlagBits GetVulkanType(const ShaderType& type);
	static shaderc_shader_kind GetShadercType(const ShaderType& type);
	static std::string GetTypeString(const ShaderType& type);
	static std::string GetTypeExtension(const ShaderType& type);
};
