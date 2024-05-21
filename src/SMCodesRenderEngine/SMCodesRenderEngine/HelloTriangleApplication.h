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
    
    void initWindow();
    
    void initVulkan();
    
    void mainLoop();
    
    void cleanUp();
    
    void createVulkanInstance();

    static std::vector<VkExtensionProperties> getVulkanExtensions();
};


#endif //SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
