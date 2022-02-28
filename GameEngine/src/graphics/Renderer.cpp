#include "precompiled.h"
#include "Renderer.h"

// TESTING FUNCTIONS //////////////////////////////////////////////////

static std::vector<char> readFile(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        LOG_ERROR("TESTING:Failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

///////////////////////////////////////////////////////////////////////


glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);

std::shared_ptr<VulkanInstance> Renderer::s_Instance;
std::shared_ptr<VulkanDevice> Renderer::s_Device;
std::shared_ptr<VulkanSwapChain> Renderer::s_SwapChain;

uint32_t Renderer::s_SwapChainWidth;
uint32_t Renderer::s_SwapChainHeight;

bool Renderer::s_ResizeRequested;


/* -- PUBLIC -- */

void Renderer::Init()
{
    s_Instance = std::make_shared<VulkanInstance>();
    VkPhysicalDevice physicalDevice = s_Instance->PickPhysicalDevice();

    s_Device = std::make_shared<VulkanDevice>(physicalDevice);
    s_Device->CreateLogicalDevice(VkPhysicalDeviceFeatures(), VULKAN_DEVICE_EXTENSIONS, nullptr);

    s_SwapChain = std::make_shared<VulkanSwapChain>(s_Instance, s_Device);
    s_SwapChain->Create(&s_SwapChainWidth, &s_SwapChainHeight);

    //createRenderPass();
    //createGraphicsPipeline();
    //createFramebuffers();
}

void Renderer::Draw()
{
    if (s_ResizeRequested)
    {
        WindowSize size = Window::GetSize();
        s_SwapChainWidth = size.Width;
        s_SwapChainHeight = size.Height;
        s_SwapChain->Create(&s_SwapChainWidth, &s_SwapChainHeight);
        s_ResizeRequested = false;
    }

}

void Renderer::CleanUp()
{
    s_SwapChain->Cleanup();
    s_Device->Cleanup();
    s_Instance->Cleanup();
}
