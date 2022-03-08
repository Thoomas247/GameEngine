#include "precompiled.h"
#include "VulkanState.h"

// ownership of these is shared accross all vulkan objects such that they are only destroyed once everything is cleaned up
std::shared_ptr<VulkanInstance> VulkanState::Instance = nullptr;
std::shared_ptr<VulkanDevice> VulkanState::Device = nullptr;
std::shared_ptr<VulkanRenderer> VulkanState::Renderer = nullptr;


/* -- PUBLIC -- */

void VulkanState::Init()
{
    Instance = std::make_shared<VulkanInstance>();
    Device = std::make_shared<VulkanDevice>(Instance->PickPhysicalDevice(), VkPhysicalDeviceFeatures(), VULKAN_DEVICE_EXTENSIONS);
    Renderer = std::make_shared<VulkanRenderer>();
}

void VulkanState::WaitIdle()
{
    vkDeviceWaitIdle(Device->LogicalDevice);
}
