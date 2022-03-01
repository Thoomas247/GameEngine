#include "precompiled.h"
#include "VulkanSwapChain.h"


/* -- PUBLIC -- */

VulkanSwapChain::VulkanSwapChain(const std::shared_ptr<VulkanInstance>& instance, const std::shared_ptr<VulkanDevice>& device)
{
	// get available queue family properties
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(device->PhysicalDevice, &queueCount, NULL);
	assert(queueCount >= 1);

	std::vector<VkQueueFamilyProperties> queueProps = std::vector<VkQueueFamilyProperties>(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device->PhysicalDevice, &queueCount, queueProps.data());

	// find a queue with present support
	std::vector<VkBool32> supportsPresent = std::vector<VkBool32>(queueCount);
	for (uint32_t i = 0; i < queueCount; i++)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(device->PhysicalDevice, i, instance->Surface, &supportsPresent[i]);
	}

	// search for a graphics queue and a present queue
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < queueCount; i++)
	{
		if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
		{
			if (graphicsQueueNodeIndex == UINT32_MAX)
			{
				graphicsQueueNodeIndex = i;
			}

			if (supportsPresent[i] == VK_TRUE)
			{
				graphicsQueueNodeIndex = i;
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	if (presentQueueNodeIndex == UINT32_MAX)
	{
		// if there is no queue that supports both present and graphics try to find a separate present queue
		for (uint32_t i = 0; i < queueCount; ++i)
		{
			if (supportsPresent[i] == VK_TRUE)
			{
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	if (graphicsQueueNodeIndex == UINT32_MAX)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Could not find a suitable graphics queue!");
	}
	if (presentQueueNodeIndex == UINT32_MAX)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Could not find a suitable present queue!");
	}

	// get list of supported surface formats
	uint32_t formatCount;

	VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(device->PhysicalDevice, instance->Surface, &formatCount, NULL);

	if (result != VK_SUCCESS || formatCount == 0)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get physical device surface format count!");
	}

	std::vector<VkSurfaceFormatKHR> surfaceFormats = std::vector<VkSurfaceFormatKHR>(formatCount);

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(device->PhysicalDevice, instance->Surface, &formatCount, surfaceFormats.data());
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get physical device surface formats!");
	}

	// if there is no preferred format we use PREFERRED_COLOR_FORMAT
	if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
	{
		ColorFormat = PREFERRED_COLOR_FORMAT;
		ColorSpace = surfaceFormats[0].colorSpace;
	}
	else
	{
		// check for the presence of PREFERRED_COLOR_FORMAT
		bool foundPreferredFormat = false;
		for (auto&& surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == PREFERRED_COLOR_FORMAT)
			{
				ColorFormat = surfaceFormat.format;
				ColorSpace = surfaceFormat.colorSpace;
				foundPreferredFormat = true;
				break;
			}
		}

		// else select the first available color format
		if (!foundPreferredFormat)
		{
			ColorFormat = surfaceFormats[0].format;
			ColorSpace = surfaceFormats[0].colorSpace;
		}
	}

	m_Instance = instance;
	m_Device = device;

	GraphicsQueueNodeIndex = graphicsQueueNodeIndex;
	PresentQueueNodeIndex = presentQueueNodeIndex;
}

VulkanSwapChain::~VulkanSwapChain()
{
	Cleanup();
}

void VulkanSwapChain::Create(VkRenderPass renderPass, uint32_t* width, uint32_t* height, bool vsync)
{
	// store the current swap chain handle so we can use it later
	VkSwapchainKHR oldSwapchain = SwapChain;

	// get physical device surface properties and formats
	VkSurfaceCapabilitiesKHR surfaceCapabilities;

	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->PhysicalDevice, m_Instance->Surface, &surfaceCapabilities);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get device surface capabilities!");
	}

	// Get available present modes
	uint32_t presentModeCount;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->PhysicalDevice, m_Instance->Surface, &presentModeCount, NULL);
	if (result != VK_SUCCESS || presentModeCount == 0)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get device surface present mode count!");
	}

	std::vector<VkPresentModeKHR> presentModes = std::vector<VkPresentModeKHR>(presentModeCount);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->PhysicalDevice, m_Instance->Surface, &presentModeCount, presentModes.data());
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get device surface present modes!");
	}

	if (surfaceCapabilities.currentExtent.width == (uint32_t)-1)
	{
		// if the surface size is undefined, the size is set to the passed size
		Extent.width = *width;
		Extent.height = *height;
	}
	else
	{
		// if the surface size is defined, the swap chain size must match
		Extent = surfaceCapabilities.currentExtent;
		*width = surfaceCapabilities.currentExtent.width;
		*height = surfaceCapabilities.currentExtent.height;
	}

	// select a present mode for the swap chain
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	// if v-sync is not requested, try to find a mailbox mode
	if (!vsync)
	{
		for (size_t i = 0; i < presentModeCount; i++)
		{
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
	}

	// determine the number of images
	uint32_t desiredNumberOfSwapchainImages = surfaceCapabilities.minImageCount + 1;
	if ((surfaceCapabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfaceCapabilities.maxImageCount))
	{
		desiredNumberOfSwapchainImages = surfaceCapabilities.maxImageCount;
	}

	// find the transformation of the surface
	VkSurfaceTransformFlagsKHR preTransform;
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surfaceCapabilities.currentTransform;
	}

	// find a supported composite alpha format
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};
	for (auto& compositeAlphaFlag : compositeAlphaFlags) 
	{
		if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag) 
		{
			compositeAlpha = compositeAlphaFlag;
			break;
		};
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = m_Instance->Surface;
	swapchainCreateInfo.minImageCount = desiredNumberOfSwapchainImages;
	swapchainCreateInfo.imageFormat = ColorFormat;
	swapchainCreateInfo.imageColorSpace = ColorSpace;
	swapchainCreateInfo.imageExtent = { Extent.width, Extent.height };
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.presentMode = swapchainPresentMode;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.compositeAlpha = compositeAlpha;

	// enable transfer source on swap chain images if supported
	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}

	// enable transfer destination on swap chain images if supported
	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	// create swap chain
	result = vkCreateSwapchainKHR(m_Device->LogicalDevice, &swapchainCreateInfo, nullptr, &SwapChain);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to create swap chain!");
	}

	if (oldSwapchain != VK_NULL_HANDLE)
	{
		for (uint32_t i = 0; i < ImageCount; i++)
		{
			vkDestroyImageView(m_Device->LogicalDevice, Buffers[i].view, nullptr);
		}
		vkDestroySwapchainKHR(m_Device->LogicalDevice, oldSwapchain, nullptr);
	}

	result = vkGetSwapchainImagesKHR(m_Device->LogicalDevice, SwapChain, &ImageCount, nullptr);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get swap chain image count!");
	}

	Images.resize(ImageCount);
	result = vkGetSwapchainImagesKHR(m_Device->LogicalDevice, SwapChain, &ImageCount, Images.data());
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to get swap chain images!");
	}

	// get the swap chain buffers
	Buffers.resize(ImageCount);
	for (uint32_t i = 0; i < ImageCount; i++)
	{
		VkImageViewCreateInfo colorAttachmentView = {};
		colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorAttachmentView.image = Images[i];
		colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorAttachmentView.format = ColorFormat;
		colorAttachmentView.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		colorAttachmentView.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		colorAttachmentView.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		colorAttachmentView.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorAttachmentView.subresourceRange.baseMipLevel = 0;
		colorAttachmentView.subresourceRange.levelCount = 1;
		colorAttachmentView.subresourceRange.baseArrayLayer = 0;
		colorAttachmentView.subresourceRange.layerCount = 1;

		Buffers[i].image = Images[i];

		result = vkCreateImageView(m_Device->LogicalDevice, &colorAttachmentView, nullptr, &Buffers[i].view);
		if (result != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to create image views!");
		}
	}

	// create framebuffers
	FrameBuffers.resize(ImageCount);
	for (size_t i = 0; i < ImageCount; i++) {
		VkImageView attachments[] = {
			Buffers[i].view
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = Extent.width;
		framebufferInfo.height = Extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_Device->LogicalDevice, &framebufferInfo, nullptr, &FrameBuffers[i]) != VK_SUCCESS)
		{
			LOG_ERROR("VULKAN_SWAP_CHAIN::Failed to create framebuffers!");
		}
	}
}

void VulkanSwapChain::Cleanup()
{
	for (int i = 0; i < FrameBuffers.size(); i++)
	{
		vkDestroyFramebuffer(m_Device->LogicalDevice, FrameBuffers[i], nullptr);
	}
	if (SwapChain != VK_NULL_HANDLE)
	{
		for (uint32_t i = 0; i < ImageCount; i++)
		{
			vkDestroyImageView(m_Device->LogicalDevice, Buffers[i].view, nullptr);
		}
	}
	if (m_Instance->Surface != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_Device->LogicalDevice, SwapChain, nullptr);
		vkDestroySurfaceKHR(m_Instance->Instance, m_Instance->Surface, nullptr);
	}
	FrameBuffers.clear();
	m_Instance->Surface = VK_NULL_HANDLE;
	SwapChain = VK_NULL_HANDLE;
}
