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

VkInstance  Renderer::s_VkInstance = VK_NULL_HANDLE;
VkPhysicalDevice Renderer::s_PhysicalDevice = VK_NULL_HANDLE;
VkDevice Renderer::s_LogicalDevice = VK_NULL_HANDLE;

VkQueue Renderer::s_GraphicsQueue = VK_NULL_HANDLE;
VkQueue Renderer::s_PresentQueue = VK_NULL_HANDLE;

VkSurfaceKHR Renderer::s_Surface = VK_NULL_HANDLE;

VkSwapchainKHR Renderer::s_SwapChain = VK_NULL_HANDLE;
std::vector<VkImage> Renderer::s_SwapChainImages;
std::vector<VkImageView> Renderer::s_SwapChainImageViews;
VkFormat Renderer::s_SwapChainImageFormat = VkFormat();
VkExtent2D Renderer::s_SwapChainExtent = VkExtent2D();

VkRenderPass Renderer::s_RenderPass = VK_NULL_HANDLE;
VkPipelineLayout Renderer::s_PipelineLayout = VK_NULL_HANDLE;
VkPipeline Renderer::s_GraphicsPipeline = VK_NULL_HANDLE;
std::vector<VkFramebuffer> Renderer::s_SwapChainFramebuffers;

VkCommandPool Renderer::s_CommandPool = VK_NULL_HANDLE;
std::vector<VkCommandBuffer> Renderer::s_CommandBuffers;

std::vector<VkSemaphore> Renderer::s_ImageAvailableSemaphores;
std::vector<VkSemaphore> Renderer::s_RenderFinishedSemaphores;
std::vector<VkFence> Renderer::s_InFlightFences;

bool Renderer::s_FrameBufferResized = false;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> c_DeviceExtensions = 
{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> c_ValidationLayers = 
{
        "VK_LAYER_KHRONOS_validation"
};


/* -- PUBLIC -- */

void Renderer::Init()
{
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSyncObjects();
}

void Renderer::Draw()
{
    static uint32_t currentFrame = 0;

    // wait for previous frame to finish
    vkWaitForFences(s_LogicalDevice, 1, &s_InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // get next image from the swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(s_LogicalDevice, s_SwapChain, UINT64_MAX, s_ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
    {
        LOG_ERROR("RENDERER::Failed to acquire swap chain image!");
    }

    vkResetFences(s_LogicalDevice, 1, &s_InFlightFences[currentFrame]);

    // record the commands we want
    vkResetCommandBuffer(s_CommandBuffers[currentFrame], 0);
    recordCommandBuffer(s_CommandBuffers[currentFrame], imageIndex);

    // submit the command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { s_ImageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &s_CommandBuffers[currentFrame];


    // set which semaphore to signal when done
    VkSemaphore signalSemaphores[] = { s_RenderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // submit draw command buffer to queue
    VkResult success = vkQueueSubmit(s_GraphicsQueue, 1, &submitInfo, s_InFlightFences[currentFrame]);

    if (success != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Failed to submit draw command buffer!");
    }

    // present the frame to the swap chain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { s_SwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // optional

    result = vkQueuePresentKHR(s_PresentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || s_FrameBufferResized) 
    {
        s_FrameBufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::CleanUp()
{
    vkDeviceWaitIdle(s_LogicalDevice);

    cleanupSwapChain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(s_LogicalDevice, s_RenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(s_LogicalDevice, s_ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(s_LogicalDevice, s_InFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(s_LogicalDevice, s_CommandPool, nullptr);

    vkDestroyDevice(s_LogicalDevice, nullptr);

    vkDestroySurfaceKHR(s_VkInstance, s_Surface, nullptr);
    vkDestroyInstance(s_VkInstance, nullptr);
}


/* -- PRIVATE -- */

void Renderer::createInstance()
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
    for (const char* layerName : c_ValidationLayers) {
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
            LOG_WARN("RENDERER::Validation layer " + std::string(layerName) + " not available");
        }
    }

    createInfo.enabledLayerCount = static_cast<uint32_t>(c_ValidationLayers.size());
    createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
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
    VkResult result = vkCreateInstance(&createInfo, nullptr, &s_VkInstance);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create vulkan instance!");
    }
    else
    {
        LOG_INFO("RENDERER::Vulkan instance created successfully");
    }
}

void Renderer::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(s_PhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.Formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.PresentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.Capabilities);

    // minimum image count required for the swap chain to function
    uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;

    if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) 
    {
        imageCount = swapChainSupport.Capabilities.maxImageCount;
    }

    // create the swap chain
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = s_Surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;    // each image is 1 layer
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(s_PhysicalDevice);
    uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

    if (indices.GraphicsFamily != indices.PresentFamily) 
    {
        //TODO: change to VK_SHARING_MODE_EXCLUSIVE once ownership chapters have been covered
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else 
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices = nullptr; // optional
    }

    createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(s_LogicalDevice, &createInfo, nullptr, &s_SwapChain);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create swap chain!");
    }
    else
    {
        LOG_INFO("RENDERER::Swap chain created successfully");
    }

    // retrieve swap chain images
    vkGetSwapchainImagesKHR(s_LogicalDevice, s_SwapChain, &imageCount, nullptr);
    s_SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(s_LogicalDevice, s_SwapChain, &imageCount, s_SwapChainImages.data());

    s_SwapChainImageFormat = surfaceFormat.format;
    s_SwapChainExtent = extent;
}

void Renderer::getAvailableExtensions()
{
    // get supported extension count
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    LOG_INFO("RENDERER::" + std::to_string(extensionCount) + " extensions supported");

    // get supported extension properties
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    LOG_INFO("RENDERER::Available extensions:");

    for (const auto& extension : extensions) 
    {
        LOG_INFO(" ->" + std::string(extension.extensionName));
    }
}

void Renderer::createSurface()
{
    VkResult result = glfwCreateWindowSurface(s_VkInstance, Window::GetWindowPtr(), nullptr, &s_Surface);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create window surface!");
    }
}

void Renderer::pickPhysicalDevice()
{
    // retrieve GPUs with vulkan support
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(s_VkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) 
    {
        LOG_ERROR("RENDERER::Failed to find GPUs with Vulkan support!");
    }

    // get available GPUs
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(s_VkInstance, &deviceCount, devices.data());

    // check if they contain the required functionality
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            s_PhysicalDevice = device;
            break;
        }
    }

    if (s_PhysicalDevice == VK_NULL_HANDLE) 
    {
        LOG_ERROR("RENDERER::Failed to find a suitable GPU!");
    }
    else
    {
        LOG_INFO("RENDERER::Physical device picked successfully");
    }

}

bool Renderer::isDeviceSuitable(VkPhysicalDevice device)
{
    /*
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    */

    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported) 
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
    }
    return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(c_DeviceExtensions.begin(), c_DeviceExtensions.end());

    for (const auto& extension : availableExtensions) 
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices Renderer::findQueueFamilies(VkPhysicalDevice device)
{
    // find all available queues (queues are used to issue commands to vulkan)
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // find a compatible queue family 
    int index = 0;
    for (const auto& queueFamily : queueFamilies) 
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
        {
            indices.GraphicsFamily = index;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, s_Surface, &presentSupport);
        if (presentSupport) 
        {
            indices.PresentFamily = index;
        }

        if (indices.IsComplete())
        {
            break;
        }
        index++;
    }


    return indices;
}

SwapChainSupportDetails Renderer::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, s_Surface, &details.Capabilities);

    // surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, s_Surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, s_Surface, &formatCount, details.Formats.data());
    }

    // surface present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, s_Surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) 
    {
        details.PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, s_Surface, &presentModeCount, details.PresentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    // find best swap surface format
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return availableFormat;
        }
    }

    // if no format is optimal, return the first one
    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    // find best swap present mode
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
        {
            return availablePresentMode;
        }
    }

    // if the optimal mode is not found, return the default mode
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    // get the size/resolution of the swap chain
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else 
    {
        WindowSize size =  Window::GetSize();

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(size.Width),
            static_cast<uint32_t>(size.Height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void Renderer::createLogicalDevice()
{
    // create queue with required capabilities
    QueueFamilyIndices indices = findQueueFamilies(s_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // set supported features, if any (eg geometry shader support)
    VkPhysicalDeviceFeatures deviceFeatures{};

    // create the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(c_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = c_DeviceExtensions.data();

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    VkResult result = vkCreateDevice(s_PhysicalDevice, &createInfo, nullptr, &s_LogicalDevice);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create logical device!");
    }
    else
    {
        LOG_INFO("RENDERER::Logical device created successfully");
    }

    vkGetDeviceQueue(s_LogicalDevice, indices.GraphicsFamily.value(), 0, &s_GraphicsQueue);
    vkGetDeviceQueue(s_LogicalDevice, indices.PresentFamily.value(), 0, &s_PresentQueue);

}

void Renderer::createImageViews()
{
    s_SwapChainImageViews.resize(s_SwapChainImages.size());

    for (size_t i = 0; i < s_SwapChainImages.size(); i++) 
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = s_SwapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = s_SwapChainImageFormat;

        // set default swizzling
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // images will be used as color targets with no mipmapping
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(s_LogicalDevice, &createInfo, nullptr, &s_SwapChainImageViews[i]);

        if (result != VK_SUCCESS) 
        {
            LOG_ERROR("RENDERER::Failed to create image views!");
        }
    }

    LOG_INFO("RENDERER::Image views created successfully");
}

void Renderer::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = s_SwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // color and depth buffer
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // clear the values to a constant at the start
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // keep framebuffer contants after rendering

    // stencil buffer
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // don't clear the values to a constant at the start
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // don't keep framebuffer contants after rendering

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;  // same as layout(location = 0) out vec4 outColor defined in the shader
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // make sure the image is available before the render pass starts
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkResult result = vkCreateRenderPass(s_LogicalDevice, &renderPassInfo, nullptr, &s_RenderPass);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create render pass!");
    }
    else
    {
        LOG_INFO("RENDERER::Render pass created successfully");
    }
}

void Renderer::createGraphicsPipeline()
{
    auto vertShaderCode = readFile("assets/shaders/VkTestVert.spv");
    auto fragShaderCode = readFile("assets/shaders/VkTestFrag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // vertex shader
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    // fragment shader
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    // set pSpecializationInfo to set constants

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)s_SwapChainExtent.width;
    viewport.height = (float)s_SwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = s_SwapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;

    rasterizer.rasterizerDiscardEnable = VK_FALSE;

    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

    rasterizer.lineWidth = 1.0f;

    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // optional
    rasterizer.depthBiasClamp = 0.0f; // optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // optional
    multisampling.pSampleMask = nullptr; // optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // optional
    multisampling.alphaToOneEnable = VK_FALSE; // optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // optional
    colorBlending.blendConstants[1] = 0.0f; // optional
    colorBlending.blendConstants[2] = 0.0f; // optional
    colorBlending.blendConstants[3] = 0.0f; // optional

    // allow certain state to be mutable
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    /*
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    */

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // optional

    VkResult result = vkCreatePipelineLayout(s_LogicalDevice, &pipelineLayoutInfo, nullptr, &s_PipelineLayout);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create pipeline layout!");
    }
    else
    {
        LOG_INFO("RENDERER::Pipeline layout created successfully");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // optional

    pipelineInfo.layout = s_PipelineLayout;

    pipelineInfo.renderPass = s_RenderPass;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
    pipelineInfo.basePipelineIndex = -1; // optional

    if (vkCreateGraphicsPipelines(s_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &s_GraphicsPipeline) != VK_SUCCESS)
    {
        LOG_ERROR("RENDERER::Failed to create graphics pipeline!");
    }
    else
    {
        LOG_INFO("RENDERER::Graphics pipeline created successfully");
    }

    vkDestroyShaderModule(s_LogicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(s_LogicalDevice, vertShaderModule, nullptr);
}

VkShaderModule Renderer::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    VkResult result = vkCreateShaderModule(s_LogicalDevice, &createInfo, nullptr, &shaderModule);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER:Failed to create shader module!");
    }

    return shaderModule;
}

void Renderer::createFramebuffers()
{
    s_SwapChainFramebuffers.resize(s_SwapChainImageViews.size());

    for (size_t i = 0; i < s_SwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            s_SwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = s_RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = s_SwapChainExtent.width;
        framebufferInfo.height = s_SwapChainExtent.height;
        framebufferInfo.layers = 1;


        VkResult result = vkCreateFramebuffer(s_LogicalDevice, &framebufferInfo, nullptr, &s_SwapChainFramebuffers[i]);

        if (result != VK_SUCCESS) 
        {
            LOG_ERROR("RENDERER::Failed to create framebuffer!");
        }
    }

    LOG_INFO("RENDERER::Framebuffers set up successfully");
}

void Renderer::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(s_PhysicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

    VkResult result = vkCreateCommandPool(s_LogicalDevice, &poolInfo, nullptr, &s_CommandPool);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create command pool!");
    }
    else
    {
        LOG_INFO("RENDERER::Command pool created successfully");
    }

}

void Renderer::createCommandBuffers()
{
    s_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = s_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)s_CommandBuffers.size();

    VkResult result = vkAllocateCommandBuffers(s_LogicalDevice, &allocInfo, s_CommandBuffers.data());

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to create command buffers!");
    }
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // optional
    beginInfo.pInheritanceInfo = nullptr; // optional

    VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

    if (result != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = s_RenderPass;
    renderPassInfo.framebuffer = s_SwapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = s_SwapChainExtent;

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s_GraphicsPipeline);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
    {
        LOG_ERROR("RENDERER::Failed to record command buffer!");
    }
}

void Renderer::createSyncObjects()
{
    s_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // set as signaled so that first frame can be rendered
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
    {

        VkResult imageAvailSuccess = vkCreateSemaphore(s_LogicalDevice, &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]);
        VkResult renderFinishedSuccess = vkCreateSemaphore(s_LogicalDevice, &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]);
        VkResult fenceSuccess = vkCreateFence(s_LogicalDevice, &fenceInfo, nullptr, &s_InFlightFences[i]);

        if (imageAvailSuccess != VK_SUCCESS || renderFinishedSuccess != VK_SUCCESS || fenceSuccess != VK_SUCCESS)
        {
            LOG_ERROR("RENDERER::Failed to create semaphores and fences!");
        }
    }

    LOG_INFO("RENDERER::Semaphores and fences created successfully");
}

void Renderer::cleanupSwapChain() 
{
    for (size_t i = 0; i < s_SwapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(s_LogicalDevice, s_SwapChainFramebuffers[i], nullptr);
    }

    vkDestroyPipeline(s_LogicalDevice, s_GraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(s_LogicalDevice, s_PipelineLayout, nullptr);
    vkDestroyRenderPass(s_LogicalDevice, s_RenderPass, nullptr);

    for (size_t i = 0; i < s_SwapChainImageViews.size(); i++) {
        vkDestroyImageView(s_LogicalDevice, s_SwapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(s_LogicalDevice, s_SwapChain, nullptr);
}

void Renderer::recreateSwapChain()
{
    // if window has been minimized, wait before recreating swap chain
    WindowSize size = Window::GetSize();
    while (size.Width == 0 || size.Height == 0)
    {
        size = Window::GetSize();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(s_LogicalDevice);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
}