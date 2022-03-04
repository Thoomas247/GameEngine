#pragma once

enum class ShaderType
{
	vertex, fragment,

	// keep this at the end
	NUM_TYPES
};

struct CachedShader
{
	ShaderType Type;
	std::string Path;

	CachedShader(const ShaderType& type, const std::string& path)
	{
		Type = type;
		Path = path;
	}
};

class VulkanPipeline
{
public:
	VkPipeline Pipeline;

private:
	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfo;
	std::vector<VkShaderModule> m_ShaderModules;
	VkPipelineLayout m_PipelineLayout;

public:
	VulkanPipeline(const std::string& glslPath);

	void Cleanup();

private:
	void getSpirvFiles(const std::string& glslPath, std::vector<CachedShader>& spirvFiles);
	std::string loadGlslFileContents(const std::string& absolutePath);
	std::string splitShader(const std::string& shaderString, const std::string& shaderType);
	std::vector<uint32_t> compileToSpirv(const std::string& shaderString, const shaderc_shader_kind& type);
	void compileFromSpirv(const std::vector<CachedShader>& spirvFiles);
	std::vector<uint32_t> loadSpirvFileContents(const std::string& absolutePath);
	void reflect(const std::vector<uint32_t>& shaderWords);
	void createPipeline();
};

