//
// Created by ShaneMonck on 21/05/2024.
//

#include "HelloTriangleApplication.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>

// #region Constants

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// #endregion

// #region Private Methods

static std::vector<char> readFile(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), (std::streamsize) fileSize);

    file.close();

    return buffer;
}


void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}


void HelloTriangleApplication::initWindow() {
    glfwInit();

    // tell glfw not to create the default OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // for now disable window resize events
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {
    createVulkanInstance();
    setupVulkanDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFrameBuffers();
}

void HelloTriangleApplication::mainLoop() {
    assert(window && "Window cannot be null");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanUp() {
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto imageView: swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(vulkanInstance, vulkanDebugMessenger, nullptr);
    }

    // note: surface must be destroyed before the instance
    vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
    vkDestroyInstance(vulkanInstance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void HelloTriangleApplication::createVulkanInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }


    // VkApplicationInfo is optional but provides us with driver information that can be useful for optimisations
    VkApplicationInfo applicationInfo = VkApplicationInfo();
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Hello Triangle";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    // Tell the Vulkan driver which global extensions and validation layers we want to sue
    VkInstanceCreateInfo createInfo = VkInstanceCreateInfo();
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;

    // Check for extension support - retrieve a list of supported extensions - unused for now
    getVulkanExtensions();

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkResult createResult = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan Instance");
    }
}


void HelloTriangleApplication::setupVulkanDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &vulkanDebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }

}


std::vector<VkExtensionProperties> HelloTriangleApplication::getVulkanExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions: \n";
    for (const auto &extension: extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    return extensions;
}

std::vector<const char *> HelloTriangleApplication::getRequiredExtensions() {
    // Use GLFW extensions to interface Vulkan with the window system
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        // enable callback to handle debug messages from Vulkan
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


bool HelloTriangleApplication::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName: validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties: availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    std::cout << "Validation layers requested are all available" << "\n";

    return true;
}

// 
VkBool32 HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                 void *pUserData) {

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        // Message is important enough to show
        switch (messageType) {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                std::cout << "Some event has happened that is unrelated to the specification or performance"
                          << std::endl;
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                std::cout << "Something has happened that violates the specification or indicates a possible mistake"
                          << std::endl;
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                std::cout << "Potential non-optimal use of Vulkan" << std::endl;
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
                std::cout
                        << "The implementation has modified the set of GPU-visible virtual addresses associated with a Vulkan object."
                        << std::endl;
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT:
                std::cout << "Not sure what this message type is, something from with flags and max enum" << std::endl;
                break;
            default:
                break;
        }

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

VkResult HelloTriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                                const VkAllocationCallbacks *pAllocator,
                                                                VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void HelloTriangleApplication::DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                             VkDebugUtilsMessengerEXT debugMessenger,
                                                             const VkAllocationCallbacks *pAllocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void HelloTriangleApplication::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU(s) with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());

    for (const auto &physDevice: physicalDevices) {
        if (isDeviceSuitable(physDevice)) {
            physicalDevice = physDevice;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU");
    }

    std::cout << "found suitable physical device " << std::endl;
}

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice physDevice) {
    // unused right now but this is how you get the physDevice properties and features
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physDevice, &deviceFeatures);

    // dedicated graphic cards that support geometry shaders
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader) {}

    // check the physDevice can process the commands we want to use
    QueueFamilyIndices indices = findQueueFamilies(physDevice);

    // check the physDevice supports all the specified required extensions
    bool extensionsSupported = checkDeviceExtensionSupport(physDevice);

    // check the physDevice adequately supports swapChain
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physDevice);
        swapChainAdequate = swapChainSupport.isAdequate();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice physDevice) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension: availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    // all required extensions available
    bool fullySupported = requiredExtensions.empty();

    if (fullySupported) {
        std::cout << "All required extensions supported" << std::endl;
    } else {
        std::cout << "Extension not supported: " << *requiredExtensions.begin() << std::endl;
    }

    return fullySupported;
}


// need to check with queue families are supported by the physDevice and which one of these
// supports the commands we want to use
HelloTriangleApplication::QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice physDevice) {
    QueueFamilyIndices indices{};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueFamilies.data());

    // need to find at least one queue family that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (const auto &queueFamily: queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        // look for a queue family that has capability of presenting to our window surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            // found what we needed
            break;
        }

        i++;
    }

    return indices;
}

void HelloTriangleApplication::createLogicalDevice() {
    // describes the number of queues we want for a single queue family
    // Right now only interested in a queue with graphics capability's
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // The currently available drivers will only allow you to create a small number of
    // queues for each queue family, and you don’t really need more than one. That’s
    // because you can create all the command buffers on multiple threads and then
    // submit them all at once on the main thread with a single low-overhead call.

    // assign priorities to queues to influence the scheduling of command buffer execution.
    // This is required even if there is only a single queue
    float queuePriority = 1.0f;
    for (uint32_t queueFamily: uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // these are the features that we queried support for with vkGetPhysicalDeviceFeatures
    // Right now don't need anything special, so leave everything with VK_FALSE
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkResult createResult = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    // retrieve queue handles
    // only creating a single queue from this family, so simply use index 0
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    std::cout << "Logical Device created and graphicsQueue and presentQueue retrieved" << std::endl;
}

void HelloTriangleApplication::createSurface() {
    VkResult createResult = glfwCreateWindowSurface(vulkanInstance, window, nullptr, &surface);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void HelloTriangleApplication::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // how many images we would like to have in the swap chain
    // it is recommended to request at least one more image than the minimum
    // to avoid waiting on the driver to complete internal operations
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    // double check did not exceed max number ( 0 is a special value indicating no maximum)
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        // Images can be used across multiple queue families without explicit ownership transfers
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        // an image is owned by one queue family at a time and ownership must be explicitly transferred
        // before using it in another queue family. This option offers the best performance
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    // if the alpha channel should be used for blending with other windows in the window system 
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // simply ignore the alpha channel

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE; // we don't care about the color of pixels that are obscured by another window in front of them

    createInfo.oldSwapchain = VK_NULL_HANDLE; // for now assume that we'll only ever create one swap chain

    VkResult createResult = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::cout << "Swap chain successfully created" << std::endl;

    // store swapChainImages
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    // store format and extent, will need in future
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

HelloTriangleApplication::SwapChainSupportDetails
HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice physDevice) {

    // 3 properties we need to check for
    // - Basic surface capabilities e.g. min/max width and height of images
    // - Surface formats e.g. pixel format, color space
    // - Available presentation modes
    SwapChainSupportDetails details;

    // Basic surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.capabilities);

    // Surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.formats.data());
    }

    // presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR
HelloTriangleApplication::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    // each VkSurfaceFormatKHR contains a format and a colorSpace member.
    // format member specifies the color channels and types
    // e.g. VK_FORMAT_B8G8R8A8_SRGB means we store the B, G, R and alpha channels in that order
    // with an 8-bit integer for a total of 32 bits
    // SRGB indicates if the SRGB color space is supported or nut using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag

    for (const auto &availableFormat: availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // if desired format is not found just default to first one (could rank them but for now just grab first)
    return availableFormats[0];
}

VkPresentModeKHR
HelloTriangleApplication::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    VkPresentModeKHR desiredPresentMode = VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;

    for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == desiredPresentMode) {
            std::cout << "Present mode: " << desiredPresentMode << " available" << std::endl;
            return availablePresentMode;
        }
    }

    std::cout << desiredPresentMode << " not available defaulting to VK_PRESENT_MODE_FIFO_KHR" << std::endl;
    // only the VK_PRESENT_MODE_FIFO_KHR mode is guaranteed to be available
    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width,
                                        capabilities.minImageExtent.width,
                                        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,
                                         capabilities.minImageExtent.height,
                                         capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void HelloTriangleApplication::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        // how the image data should be interpreted
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        // components field allows you to swizzle the color channels around
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // subresourceRange field describes what the image's purpose is 
        // our images will be used as color targets without any mipmapping levels or multiple layers
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult createResult = vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]);
        if (createResult != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

void HelloTriangleApplication::createRenderPass() {
    // Attachment Description
    // just a single color buffer attachment represented by one of the images from the swap chain
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // no multisampling right now
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;  // clear the values to a constant at the start
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Rendered contents will be stored in memory and can be read later
    // our app won't do anything with stencil buffer so loading and storing are irrelevant
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // Existing contents are undefined; we don't care about them
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // Contents of the framebuffer will be undefined after the rendering operation
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // want the image to be ready for presentation using the swap chain after rendering

    // Subpasses (post-processing) and attachment references 
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0; // just one VkAttachmentDescription
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // our attachment is a color buffer

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    // the index of the attachment in this array is directly referenced
    // from the fragment shader with the layout(location = 0) out vec4 outColor

    // Render pass
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkResult renderPassCreateResult = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
    if (renderPassCreateResult != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void HelloTriangleApplication::createGraphicsPipeline() {
    auto vertexShader = readFile("shaders/hello_triangle_application.vert.spv");
    auto fragmentShader = readFile("shaders/hello_triangle_application.frag.spv");

    std::cout << "VertexShader Size: " << vertexShader.size() << std::endl;
    std::cout << "FragmentShader Size: " << fragmentShader.size() << std::endl;

    VkShaderModule vertShaderModule = createShaderModule(vertexShader);
    VkShaderModule fragShaderModule = createShaderModule(fragmentShader);

    // assign shaders to specific pipeline stage
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;  // vertex stage
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.pSpecializationInfo = nullptr; // Optional - Allows to specify values for shader constants

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;// fragment stage
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";
    fragShaderStageInfo.pSpecializationInfo = nullptr; // Optional

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                      fragShaderStageInfo};


    // Vertex Input (for now vertex data is hardcoded in shader)
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    // Input Assembly ( what kind of geometry will be drawn from the vertices
    // and if primitive restart should be enabled)
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // triangle from every 3 vertices without reuse
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewports and scissors ( region of the framebuffer that the output will be rendered to)
    // usually always (0,0) to (width, height)
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    //viewportState.pViewports = &viewport; // this would be null if using dynamic states
    viewportState.scissorCount = 1;
    //viewportState.pScissors = &scissor; // null if using dynaimc states

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE; // discard fragments beyond near and far planes don't clamp to edge
    rasterizer.rasterizerDiscardEnable = VK_FALSE; // if true disables any output to the frame-buffer
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // fill the area of the polygon with fragments
    rasterizer.lineWidth = 1.0f; // thickness of lines
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; // cull the back faces
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // vertex order of faces
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional


    // Multisampling (anti-aliasing) for now disabled
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // Depth and stencil testing (not implemented)
    // ...

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; // new color from the fragment shader is passed through unmodified
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlending{}; // fragment colors will be written to the framebuffer unmodified
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; //optional
    colorBlending.blendConstants[1] = 0.0f; //optional
    colorBlending.blendConstants[2] = 0.0f; //optional
    colorBlending.blendConstants[3] = 0.0f; //optional


    // states that can change without having to recreate the pipeline at draw time
    // meaning we will be required to specify the data for these values at drawing time
    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data(); 
    
    // Pipeline Layout (for uniform values in shaders)
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    pipelineLayoutInfo.pNext = nullptr;  // had to add this? not sure why
    pipelineLayoutInfo.flags = VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT; // had to also add this?

    VkResult pipelineLayoutCreateResult = vkCreatePipelineLayout(device,
                                                                 &pipelineLayoutInfo,
                                                                 nullptr,
                                                                 &pipelineLayout);
    if (pipelineLayoutCreateResult != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }
    
    std::cout<< "Successfully created pipeline layout"<< std::endl;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0; // index of the sub pass where this graphics pipeline will be used
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional - if wanted a new graphics pipeline derived from an exisiting pipeline
    pipelineInfo.basePipelineIndex = -1; //Optional

    VkResult pipelineCreateResults = vkCreateGraphicsPipelines(device,
                                                               VK_NULL_HANDLE,
                                                               1,
                                                               &pipelineInfo,
                                                               nullptr,
                                                               &graphicsPipeline);
    if (pipelineCreateResults != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    std::cout << "Successfully created Graphics Pipeline" << std::endl;

    // can destroy shader modules after pipeline creation is finished
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
}

VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char> &shaderCode) {

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

    VkShaderModule shaderModule;
    VkResult createResult = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);

    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }

    return shaderModule;
}


// #endregion

// #region Public Methods
void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}













// #endregion
