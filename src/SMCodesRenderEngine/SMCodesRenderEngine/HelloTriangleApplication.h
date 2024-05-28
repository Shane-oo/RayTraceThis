//
// Created by ShaneMonck on 21/05/2024.
//

#ifndef SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
#define SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H


#include <vulkan/vulkan_core.h>
#include <vector>
#include <optional>

class GLFWwindow;

class HelloTriangleApplication {
    
    

public:
    void run();

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        
        bool isComplete() const{
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    GLFWwindow *window;
    VkInstance vulkanInstance;
    VkDebugUtilsMessengerEXT vulkanDebugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkQueue presentQueue;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanUp();

    void createVulkanInstance();

    void setupVulkanDebugMessenger();

    static std::vector<VkExtensionProperties> getVulkanExtensions();

    static std::vector<const char *> getRequiredExtensions();

    static bool checkValidationLayerSupport();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    // could probs be made global
    static VkResult
    CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                 const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

    // could probs be made global
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator);

    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);
    
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    
    void createLogicalDevice();
    
    void createSurface();
};


#endif //SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
