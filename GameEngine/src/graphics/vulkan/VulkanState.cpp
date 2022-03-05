#include "precompiled.h"
#include "VulkanState.h"


std::unique_ptr<VulkanInstance> VulkanState::Instance = nullptr;
std::unique_ptr<VulkanDevice> VulkanState::Device = nullptr;
std::unique_ptr<VulkanRenderer> VulkanState::Renderer = nullptr;

// TODO: find a better way to Cleanup() these at the end
std::vector<VulkanPipeline*> VulkanState::Pipelines;
std::vector<VulkanBuffer*> VulkanState::Buffers;


/* -- PUBLIC -- */

void VulkanState::Init()
{
    Instance = std::make_unique<VulkanInstance>();
    Device = std::make_unique<VulkanDevice>(Instance->PickPhysicalDevice(), VkPhysicalDeviceFeatures(), VULKAN_DEVICE_EXTENSIONS);
    Renderer = std::make_unique<VulkanRenderer>();
}

void VulkanState::Cleanup()
{
    vkDeviceWaitIdle(Device->LogicalDevice);

    for (int i = 0; i < Pipelines.size(); i++)
    {
        Pipelines[i]->Cleanup();
    }
    for (int i = 0; i < Buffers.size(); i++)
    {
        Buffers[i]->Cleanup();
    }
    Renderer->Cleanup();
    Device->Cleanup();
    Instance->Cleanup();
}
