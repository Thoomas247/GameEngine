#pragma once

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

	std::vector<VkShaderModule> m_ShaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;

	VkPipelineLayout m_PipelineLayout;

public:
	VulkanPipeline() = default;
	VulkanPipeline(VulkanPipeline&& oldPipeline) noexcept;
	~VulkanPipeline();

	void Init(const std::vector<SpirvCodeInfo>& spirvCode, VkDescriptorSetLayout descriptorSetLayout, const uint32_t& setLayoutCount);

private:
	void compileShaderStages(const std::vector<SpirvCodeInfo>& spirvCode);
	void createPipeline(VkDescriptorSetLayout descriptorSetLayout, const uint32_t& setLayoutCount);
};

