#pragma once

/// <summary>
/// Struct used to represent a cached shader which needs to be loaded.
/// </summary>
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

// TODO: Move shader file loading to another class. VulkanPipeline should only handle compiling and descriptor layouts.

/// <summary>
/// The pipeline combines the shader and material into one object as the two need eachother to work.
/// </summary>
class VulkanPipeline
{
public:
	VkPipeline Pipeline = VK_NULL_HANDLE;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfo;
	std::vector<VkShaderModule> m_ShaderModules;

	VkPipelineLayout m_PipelineLayout;

	std::vector<VkDescriptorSetLayoutBinding> m_LayoutBindings;
	VkDescriptorSetLayout m_DescriptorSetLayout;

public:
	VulkanPipeline() = default;
	VulkanPipeline(VulkanPipeline&& oldPipeline) noexcept;
	~VulkanPipeline();

	void Init(const std::string& glslPath);

private:
	/// <summary>
	/// Retrieves the spirv files of this shader.
	/// If the shader hasn't been cached yet, the shader is converted to spirv and saved automatically.
	/// </summary>
	/// <param name="glslPath"></param>
	/// <param name="spirvFiles">The vector to fill in with the required information about this shader's spirv files</param>
	void getSpirvFiles(const std::string& glslPath, std::vector<CachedShader>& spirvFiles);
	void compileFromSpirvAndReflect(const std::vector<CachedShader>& spirvFiles);
	void createPipeline();
};

