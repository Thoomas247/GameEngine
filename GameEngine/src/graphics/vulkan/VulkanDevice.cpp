#include "precompiled.h"
#include "StructsAndConstants.h"
#include "VulkanDevice.h"


/* -- PUBLIC -- */

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions)
{
	if (physicalDevice == nullptr)
	{
		LOG_ERROR("VULKAN_DEVICE::Physical device cannot be null!");
	}

	if (VulkanState::Instance == nullptr)
	{
		LOG_ERROR("VULKAN_DEVICE::Instance must be created before creating the device!");
	}

	// store properties, features, limits and properties of the physical device for later use
	vkGetPhysicalDeviceProperties(physicalDevice, &Properties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &Features);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &MemoryProperties);

	// get queue properties
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	if (queueFamilyCount <= 0)
	{
		LOG_ERROR("VULKAN_DEVICE::Could not retrieve the physical device's queue families!");
	}
	QueueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, QueueFamilyProperties.data());


	// get list of supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	if (extensionCount > 0)
	{
		std::vector<VkExtensionProperties> extensions = std::vector<VkExtensionProperties>(extensionCount);
		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, &extensions.front()) == VK_SUCCESS)
		{
			for (const auto& extension : extensions)
			{
				SupportedExtensions.push_back(extension.extensionName);
			}
		}
	}

	PhysicalDevice = physicalDevice;
	EnabledFeatures = enabledFeatures;

	createLogicalDevice(VULKAN_DEVICE_EXTENSIONS, nullptr);
	createCommandPool(QueueFamilyIndices.graphics);

	vkGetDeviceQueue(LogicalDevice, QueueFamilyIndices.graphics, 0, &GraphicsQueue);
	vkGetDeviceQueue(LogicalDevice, QueueFamilyIndices.compute, 0, &ComputeQueue);
	vkGetDeviceQueue(LogicalDevice, QueueFamilyIndices.transfer, 0, &TransferQueue);
}

VulkanDevice::~VulkanDevice()
{
	if (CommandPool)
	{
		vkDestroyCommandPool(LogicalDevice, CommandPool, nullptr);
	}
	if (LogicalDevice)
	{
		vkDestroyDevice(LogicalDevice, nullptr);
	}
}

void VulkanDevice::Cleanup()
{
	vkDestroyCommandPool(LogicalDevice, CommandPool, nullptr);
	vkDestroyDevice(LogicalDevice, nullptr);

	CommandPool = VK_NULL_HANDLE;
	LogicalDevice = VK_NULL_HANDLE;
}

uint32_t VulkanDevice::GetMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound)
{
	for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				if (memTypeFound)
				{
					*memTypeFound = true;
				}
				return i;
			}
		}
		typeBits >>= 1;
	}

	if (memTypeFound)
	{
		*memTypeFound = false;
		return 0;
	}
	else
	{
		LOG_ERROR("VULKAN_DEVICE::Could not find matching memory type!");
	}

}

uint32_t VulkanDevice::GetQueueFamilyIndex(VkQueueFlagBits queueFlags)
{
	// dedicated compute queue (find index which supports compute but not graphics)
	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(QueueFamilyProperties.size()); i++)
		{
			if ((QueueFamilyProperties[i].queueFlags & queueFlags) && ((QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;	// return the index of this queue if it matches the requirements
			}
		}
	}

	// dedicated queue for transfer
	if (queueFlags & VK_QUEUE_TRANSFER_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(QueueFamilyProperties.size()); i++)
		{
			if ((QueueFamilyProperties[i].queueFlags & queueFlags) && ((QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) 
				&& ((QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				return i;	// return the index of this queue if it matches the requirements
			}
		}
	}

	// return any queue with the requirements if specialized queues cannot be found
	for (uint32_t i = 0; i < static_cast<uint32_t>(QueueFamilyProperties.size()); i++)
	{
		if (QueueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
		}
	}

	LOG_ERROR("VULKAN_DEVICE::Could not find a suitable queue family index!");
}

VkResult VulkanDevice::CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data)
{
	// create buffer handle
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.size = size;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(LogicalDevice, &bufferCreateInfo, nullptr, buffer);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Failed to create buffer!");
	}

	// allocate memory for the buffer
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(LogicalDevice, *buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo{};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = GetMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags);

	// enable required flags before allocation
	VkMemoryAllocateFlagsInfoKHR allocateFlagsInfo{};
	if (usageFlags & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
	{
		allocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
		allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
		memoryAllocateInfo.pNext = &allocateFlagsInfo;
	}

	result = vkAllocateMemory(LogicalDevice, &memoryAllocateInfo, nullptr, memory);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Failed to allocate buffer memory!");
	}

	// fill the buffer with the passed data
	if (data != nullptr)
	{
		void* mapped;
		result = vkMapMemory(LogicalDevice, *memory, 0, size, 0, &mapped);
		if (result != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_DEVICE::Failed to map data to buffer!");
		}
		memcpy(mapped, data, size);

		// manually flush if VK_MEMORY_PROPERTY_HOST_COHERENT_BIT hasn't been enabled 
		if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
		{
			VkMappedMemoryRange range{};
			range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			range.memory = *memory;
			range.offset = 0;
			range.size = size;
			vkFlushMappedMemoryRanges(LogicalDevice, 1, &range);
		}
		vkUnmapMemory(LogicalDevice, *memory);
	}

	// attach the memory to the buffer
	result = vkBindBufferMemory(LogicalDevice, *buffer, *memory, 0);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Failed to bind memory to buffer!");
	}

	return VK_SUCCESS;
}

VkCommandBuffer VulkanDevice::CreateCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = pool;
	commandBufferAllocateInfo.level = level;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VkResult result = vkAllocateCommandBuffers(LogicalDevice, &commandBufferAllocateInfo, &commandBuffer);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Failed to allocate command buffer!");
	}

	// if we should begin recording
	if (begin)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if (result != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_DEVICE::Failed to begin command buffer recording!");
		}
	}

	return commandBuffer;
}

VkCommandBuffer VulkanDevice::CreateCommandBuffer(VkCommandBufferLevel level, bool begin)
{
	return CreateCommandBuffer(level, CommandPool, begin);
}

bool VulkanDevice::ExtensionSupported(const std::string& extension)
{
	return (std::find(SupportedExtensions.begin(), SupportedExtensions.end(), extension) != SupportedExtensions.end());
}

VkFormat VulkanDevice::GetSupportedDepthFormat(bool checkSamplingSupport)
{
	std::vector<VkFormat> depthFormats = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D16_UNORM };

	for (const auto& format : depthFormats)
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(PhysicalDevice, format, &formatProperties);

		// format must support depth stencil for optimal tiling
		if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			if (checkSamplingSupport)
			{
				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT))
				{
					continue;
				}
			}
			return format;
		}
	}

	LOG_ERROR("VULKAN_DEVICE::Could not find a matching depth format!");
}


/* -- PRIVATE -- */

void VulkanDevice::createLogicalDevice(std::vector<const char*> enabledExtensions, void* pNextChain, bool useSwapChain, VkQueueFlags requestedQueueTypes)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	const float defaultQueuePriority = 0.0f;

	// graphics queue
	if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
	{
		QueueFamilyIndices.graphics = GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = QueueFamilyIndices.graphics;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	else
	{
		QueueFamilyIndices.graphics = 0;
	}

	// compute queue
	if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
	{
		QueueFamilyIndices.compute = GetQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
		if (QueueFamilyIndices.compute != QueueFamilyIndices.graphics)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = QueueFamilyIndices.compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		QueueFamilyIndices.compute = QueueFamilyIndices.graphics;
	}

	// transfer queue
	if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
	{
		QueueFamilyIndices.transfer = GetQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
		if ((QueueFamilyIndices.transfer != QueueFamilyIndices.graphics) && (QueueFamilyIndices.transfer != QueueFamilyIndices.compute))
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = QueueFamilyIndices.transfer;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		QueueFamilyIndices.transfer = QueueFamilyIndices.graphics;
	}

	// create logical device
	std::vector<const char*> deviceExtensions = std::vector<const char*>(enabledExtensions);
	if (useSwapChain)
	{
		// if the device will be used to present to a display we need to request the swapchain extension
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &EnabledFeatures;

	VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
	if (pNextChain)
	{
		physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		physicalDeviceFeatures2.features = EnabledFeatures;
		physicalDeviceFeatures2.pNext = pNextChain;
		deviceCreateInfo.pEnabledFeatures = nullptr;
		deviceCreateInfo.pNext = &physicalDeviceFeatures2;
	}

	if (ExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
	{
		deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
		m_EnableDebugMarkers = true;
	}

	if (deviceExtensions.size() > 0)
	{
		for (const char* enabledExtension : deviceExtensions)
		{
			if (!ExtensionSupported(enabledExtension))
			{
				LOG_ERROR("VULKAN_DEVICE::Enabled device extension \"" + std::string(enabledExtension) + "\" is not present at device level!");
			}
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	}

	VkResult result = vkCreateDevice(PhysicalDevice, &deviceCreateInfo, nullptr, &LogicalDevice);

	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Failed to create logical device!");
	}
}

void VulkanDevice::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
{
	VkCommandPoolCreateInfo poolCreateInfo{};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	poolCreateInfo.flags = createFlags;

	if (vkCreateCommandPool(LogicalDevice, &poolCreateInfo, nullptr, &CommandPool) != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_DEVICE::Could not create command pool!");
	}

}
