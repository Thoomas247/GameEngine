#pragma once

class VulkanInstance
{
public:
	VkInstance Instance;
	VkSurfaceKHR Surface;

public:
	VulkanInstance();
	~VulkanInstance();

	void Cleanup();

	VkPhysicalDevice PickPhysicalDevice();

private:
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
};

