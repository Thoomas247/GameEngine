#pragma once

class VulkanDescriptorPool
{
public:
	VkDescriptorSetLayout DescriptorSetLayout;

private:
	std::shared_ptr<VulkanInstance> m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;

	VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
	std::vector<VkDescriptorSet> m_DescriptorSets;

public:
	VulkanDescriptorPool() = default;
	VulkanDescriptorPool(VulkanDescriptorPool&& oldDescriptorPool) noexcept;
	~VulkanDescriptorPool();

	void Init(const std::vector<DescriptorPoolSize>& descriptorPoolInfo, const std::vector<DescriptorSetBinding>& descriptorSetBindings);

private:
	void allocatePool(const std::vector<DescriptorPoolSize>& descriptorPoolInfo);
	void allocateDescriptors(const std::vector<DescriptorSetBinding>& descriptorSetBindings);
};

