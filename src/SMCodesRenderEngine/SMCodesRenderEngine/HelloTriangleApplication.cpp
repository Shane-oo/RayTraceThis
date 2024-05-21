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

// #region Constants

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// #endregion

// #region Private Methods
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
}

void HelloTriangleApplication::mainLoop() {
    assert(window && "Window cannot be null");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanUp() {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(vulkanInstance, vulkanDebugMessenger, nullptr);
    }

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

// #endregion

// #region Public Methods
void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}

// #endregion
