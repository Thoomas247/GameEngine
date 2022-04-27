#pragma once


/* -- ENUMS -- */

enum class ShaderType
{
	vertex, fragment,

	// keep this at the end
	NUM_TYPES
};

enum class DescriptorType
{
	uniform_buffer,

	// keep this at the end
	NUM_TYPES
};


/* -- STRUCTS -- */

/// <summary>
/// Struct used to represent a cached shader which needs to be loaded.
/// </summary>
struct CachedShaderInfo
{
	ShaderType Type;
	std::string Path;

	CachedShaderInfo(const ShaderType& type, const std::string& path)
	{
		Type = type;
		Path = path;
	}
};

/// <summary>
/// Used to pass the shader code to the pipeline.
/// </summary>
struct SpirvCodeInfo
{
	ShaderType Type;
	std::vector<uint32_t> Code;
};

struct DescriptorPoolSize
{
	DescriptorType Type;
	uint32_t Size;
};

struct DescriptorSetBinding
{
	uint32_t Binding;
	DescriptorType Type;
	uint32_t Count;
	ShaderType Stage;
};


/* -- UTIL -- */

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
	static VkDescriptorType GetVulkanType(const DescriptorType& type);

	static shaderc_shader_kind GetShadercType(const ShaderType& type);

	static std::string GetTypeString(const ShaderType& type);
	static std::string GetTypeString(const DescriptorType& type);

	static std::string GetTypeExtension(const ShaderType& type);
};
