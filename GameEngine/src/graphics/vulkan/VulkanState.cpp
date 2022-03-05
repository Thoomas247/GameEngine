#include "precompiled.h"
#include "VulkanState.h"


std::unique_ptr<VulkanInstance> VulkanState::Instance = nullptr;
std::unique_ptr<VulkanDevice> VulkanState::Device = nullptr;
std::unique_ptr<VulkanSwapChain> VulkanState::SwapChain = nullptr;
std::unique_ptr<VulkanRenderPass> VulkanState::RenderPass = nullptr;

// TODO: store vulkan's pointers directly instead of objects
//       this way the objects can be deleted but the vk pointers will remain accessible
std::vector<VulkanPipeline*> VulkanState::Pipelines;
std::vector<VulkanBuffer*> VulkanState::Buffers;


/* -- PUBLIC -- */

void VulkanState::Init()
{
    Instance = std::make_unique<VulkanInstance>();
    Device = std::make_unique<VulkanDevice>(Instance->PickPhysicalDevice(), VkPhysicalDeviceFeatures(), VULKAN_DEVICE_EXTENSIONS);
    SwapChain = std::make_unique<VulkanSwapChain>();
    RenderPass = std::make_unique<VulkanRenderPass>();
}

void VulkanState::Cleanup()
{
    for (int i = 0; i < Pipelines.size(); i++)
    {
        Pipelines[i]->Cleanup();
    }
    for (int i = 0; i < Buffers.size(); i++)
    {
        Buffers[i]->Cleanup();
    }
    RenderPass->Cleanup();
    SwapChain->Cleanup();
    Device->Cleanup();
    Instance->Cleanup();
}
