#include "precompiled.h"
#include "VulkanDescriptorPool.h"


/* -- PUBLIC -- */

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& oldDescriptorPool) noexcept
{
	DescriptorSetLayout = oldDescriptorPool.DescriptorSetLayout;

	m_Instance = oldDescriptorPool.m_Instance;
	m_Device = oldDescriptorPool.m_Device;

	m_DescriptorPool = oldDescriptorPool.m_DescriptorPool;
	m_DescriptorSets = oldDescriptorPool.m_DescriptorSets;

	oldDescriptorPool.m_DescriptorPool = VK_NULL_HANDLE;
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
	if (m_DescriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(m_Device->LogicalDevice, m_DescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(m_Device->LogicalDevice, DescriptorSetLayout, nullptr);
	}

	m_DescriptorPool = VK_NULL_HANDLE;
}

void VulkanDescriptorPool::Init(const std::vector<DescriptorPoolSize>& descriptorPoolInfo, const std::vector<DescriptorSetBinding>& descriptorSetBindings)
{
	m_Instance = VulkanState::Instance;
	m_Device = VulkanState::Device;

	allocatePool(descriptorPoolInfo);
	allocateDescriptors(descriptorSetBindings);
}


/* -- PRIVATE -- */

void VulkanDescriptorPool::allocatePool(const std::vector<DescriptorPoolSize>& descriptorPoolInfo)
{
	// create descriptor pool
	std::vector<VkDescriptorPoolSize> poolSizes;

	uint32_t totalPoolSize = 0;

	for (uint32_t i = 0; i < descriptorPoolInfo.size(); i++)
	{
		if (descriptorPoolInfo[i].Size > 0)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = ShaderUtil::GetVulkanType(descriptorPoolInfo[i].Type);
			poolSize.descriptorCount = descriptorPoolInfo[i].Size;
			poolSizes.push_back(poolSize);

			totalPoolSize += descriptorPoolInfo[i].Size;
		}
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = totalPoolSize;	// max number of descriptors that can be allocated across all types.

	if (vkCreateDescriptorPool(m_Device->LogicalDevice, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DESCRIPTOR_POOL::Failed to create descriptor pool!");
	}
}

void VulkanDescriptorPool::allocateDescriptors(const std::vector<DescriptorSetBinding>& descriptorSetBindings)
{
	uint32_t descriptorSetCount = descriptorSetBindings.size();

	// create layout bindings
	std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
	descriptorSetLayoutBindings.resize(descriptorSetCount);

	for (uint32_t i = 0; i < descriptorSetCount; i++)
	{
		descriptorSetLayoutBindings[i].binding = descriptorSetBindings[i].Binding;
		descriptorSetLayoutBindings[i].descriptorCount = descriptorSetBindings[i].Count;
		descriptorSetLayoutBindings[i].descriptorType = ShaderUtil::GetVulkanType(descriptorSetBindings[i].Type);
		descriptorSetLayoutBindings[i].stageFlags = ShaderUtil::GetVulkanType(descriptorSetBindings[i].Stage);
	}

	// create descriptor set layout 
	VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = descriptorSetCount;
	layoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(m_Device->LogicalDevice, &layoutCreateInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DESCRIPTOR_POOL::Failed to create descriptor set layout!");
	}

	// allocate descriptors
	std::vector<VkDescriptorSetLayout> layouts(descriptorSetCount, DescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = descriptorSetCount;
	allocInfo.pSetLayouts = layouts.data();

	m_DescriptorSets.resize(descriptorSetCount);
	if (vkAllocateDescriptorSets(m_Device->LogicalDevice, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DESCRIPTOR_POOL::Failed to allocate descriptor sets!");
	}
}
