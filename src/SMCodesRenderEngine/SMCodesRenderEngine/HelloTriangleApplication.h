//
// Created by ShaneMonck on 21/05/2024.
//

#ifndef SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
#define SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H


#include <vulkan/vulkan_core.h>
#include <vector>
#include <optional>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <array>
#include <glm/ext/matrix_float4x4.hpp>

class GLFWwindow;

class HelloTriangleApplication {


public:
    void run();

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        bool isAdequate() const {
            return !formats.empty() && !presentModes.empty();
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
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;
    bool frameBufferResized = false;

    void initWindow();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

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

    bool isDeviceSuitable(VkPhysicalDevice physDevice);

    static bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physDevice);

    void createLogicalDevice();

    void createSurface();

    void createSwapChain();

    void recreateSwapChain();

    void cleanupSwapChain();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physDevice);

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void createImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    VkShaderModule createShaderModule(const std::vector<char> &shaderCode);

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer cmdBuffer, uint32_t imageIndex);

    void drawFrame();

    void createSyncObjects();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createBuffer(VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 colour;


        static VkVertexInputBindingDescription getBindingDescription() {
            // describes at which rate to load data from memory throughout the vertices.
            // Specifies the number of bytes between data entries and whether
            // to move to the next data entry after each vertex or after each instance
            VkVertexInputBindingDescription bindingDescription{};

            bindingDescription.binding = 0; // index of the binding in the array of bindings
            bindingDescription.stride = sizeof(Vertex); // the number of bytes from one entry to the next
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // Move to the next data entry after each vertex


            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            // describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
            // we have two attributes, position and color, so we need two attribute description structs
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            // position
            attributeDescriptions[0].binding = 0; // which binding the per-vertex data comes
            attributeDescriptions[0].location = 0; // references the location directive of the input in the vertex shader
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            // the number of bytes since the start of the per-vertex data to read from
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            // colour
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, colour);

            return attributeDescriptions;
        }
    };

    const std::vector<Vertex> vertices = {
            // rectangle
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // top-left
            {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},  // top-right
            {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}}, // bottom-right
            {{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}},  // bottom-left
    };
    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createVertexBuffer();

    void createIndexBuffer();
    
private: 
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    void createDescriptorSetLayout();
    
    void createUniformBuffers();
    
    void updateUniformBuffer(uint32_t currentImage);
};


#endif //SMCODESRENDERENGINE_HELLOTRIANGLEAPPLICATION_H
