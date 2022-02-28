#pragma once

struct QueueIndices
{
	uint32_t graphics;
	uint32_t compute;
	uint32_t transfer;
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChainBuffer
{
	VkImage image;
	VkImageView view;
};

constexpr auto PREFERRED_COLOR_FORMAT = VK_FORMAT_B8G8R8A8_UNORM;

const std::vector<const char*> VULKAN_VALIDATION_LAYERS = {
		"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> VULKAN_DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
};