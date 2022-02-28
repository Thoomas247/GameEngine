#include "precompiled.h"
#include "VulkanInstance.h"


/* -- PUBLIC -- */

VulkanInstance::VulkanInstance()
{
    // application info (optional)
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Game Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // required info to create instance
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // get available validation layers
#ifndef NDEBUG
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // check if all the layers we want are available
    for (const char* layerName : VULKAN_VALIDATION_LAYERS) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            LOG_WARN("VULKAN_INSTANCE::Validation layer " + std::string(layerName) + " not available");
        }
    }

    createInfo.enabledLayerCount = static_cast<uint32_t>(VULKAN_VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VULKAN_VALIDATION_LAYERS.data();
#else
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
#endif // !NDEBUG

    // get glfw extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // create instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &Instance);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_INSTANCE::Failed to create vulkan instance!");
    }
    else
    {
        LOG_INFO("VULKAN_INSTANCE::Vulkan instance created successfully");
    }

    // create surface
    result = Window::GetWindowSurfaceForVulkan(Instance, &Surface);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("VULKAN_INSTANCE::Failed to create window surface!");
    }
}

VulkanInstance::~VulkanInstance()
{
    if (Instance)
    {
        vkDestroyInstance(Instance, nullptr);
    }
}

void VulkanInstance::Cleanup()
{
    vkDestroyInstance(Instance, nullptr);
    Instance = VK_NULL_HANDLE;
}

VkPhysicalDevice VulkanInstance::PickPhysicalDevice()
{
    // retrieve GPUs with vulkan support
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        LOG_ERROR("VULKAN_INSTANCE::Failed to find GPUs with Vulkan support!");
    }

    // get available GPUs
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    // check if they contain the required functionality
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        LOG_ERROR("VULKAN_INSTANCE::Failed to find a suitable GPU!");
    }
    
    return physicalDevice;
}


/* -- PRIVATE -- */

bool VulkanInstance::isDeviceSuitable(VkPhysicalDevice device)
{
    /*
     VkPhysicalDeviceProperties deviceProperties;
     vkGetPhysicalDeviceProperties(device, &deviceProperties);

     VkPhysicalDeviceFeatures deviceFeatures;
     vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    */

    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return extensionsSupported && swapChainAdequate;
}

bool VulkanInstance::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(VULKAN_DEVICE_EXTENSIONS.begin(), VULKAN_DEVICE_EXTENSIONS.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails VulkanInstance::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &details.capabilities);

    // surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, nullptr);

    if (formatCount != 0) 
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, details.formats.data());
    }

    // surface present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

