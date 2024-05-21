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
// #endregion

// #region Private Methods

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
}

void HelloTriangleApplication::mainLoop() {
    assert(window && "Window cannot be null");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanUp() {
    vkDestroyInstance(vulkanInstance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void HelloTriangleApplication::createVulkanInstance() {
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

    // Use GLFW extensions to interface Vulkan with the window system
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VkResult createResult = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
    if (createResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan Instance");
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


// #endregion

// #region Public Methods
void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}





// #endregion
