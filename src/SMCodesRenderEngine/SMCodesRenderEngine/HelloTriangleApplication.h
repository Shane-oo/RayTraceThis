//
// Created by ShaneMonck on 21/05/2024.
//

#ifndef SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
#define SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H


#include <vulkan/vulkan_core.h>
#include <vector>

class GLFWwindow;

class HelloTriangleApplication {
public:
    void run();

private:
    GLFWwindow* window;
    VkInstance vulkanInstance;
    VkDebugUtilsMessengerEXT vulkanDebugMessenger;
    
    void initWindow();
    
    void initVulkan();
    
    void mainLoop();
    
    void cleanUp();
    
    void createVulkanInstance();
    
    void setupVulkanDebugMessenger();

    static std::vector<VkExtensionProperties> getVulkanExtensions();
    
    std::vector<const char*> getRequiredExtensions();
    
    bool checkValidationLayerSupport();
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT  messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT  messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, 
                                                        void* pUserData);
    
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    
    // could probs be made global
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    // could probs be made global
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator);
};


#endif //SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
