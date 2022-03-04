#pragma once

class ShaderUtil
{
public:
	static VkShaderStageFlagBits GetVulkanType(const ShaderType& type);
	static shaderc_shader_kind GetShadercType(const ShaderType& type);
	static std::string GetTypeString(const ShaderType& type);
	static std::string GetTypeExtension(const ShaderType& type);
};
