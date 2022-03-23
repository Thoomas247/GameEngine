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
	/// <summary>
	/// Returns a string containing the shader file's content
	/// </summary>
	/// <param name="absolutePath">The path of the glsl file</param>
	/// <returns></returns>
	static std::string LoadGlslFileContents(const std::string& absolutePath);
	/// <summary>
	/// Returns a string representing the shader code of a single stage (eg vertex, fragment, etc...).
	/// The passed shader string must have #start and #end tags representing where each stage starts and ends.
	/// </summary>
	/// <param name="shaderString">The shader to split</param>
	/// <param name="shaderType"></param>
	/// <returns></returns>
	static std::string SplitShaderStage(const std::string& shaderString, const ShaderType& shaderType);
	/// <summary>
	/// Converts a glsl shader into spirv code.
	/// </summary>
	/// <param name="shaderString">The shader stage glsl code to convert</param>
	/// <param name="shaderType">The type of shader being converted (eg vertex, fragment, etc...)</param>
	/// <returns></returns>
	static std::vector<uint32_t> ConvertToSpirv(const std::string& shaderString, const ShaderType& shaderType);

	static VkShaderStageFlagBits GetVulkanType(const ShaderType& type);
	static shaderc_shader_kind GetShadercType(const ShaderType& type);
	static std::string GetTypeString(const ShaderType& type);
	static std::string GetTypeExtension(const ShaderType& type);
};
