# CMakeList.txt : CMake project for SMCodesRenderEngine, include source and define
# project specific logic here.
#

# Copy the shaders folder to the build and binary directory 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Copy the textures folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Copy the models folder to the build and binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add source to this project's executable.
add_executable(SMCodesRenderEngine "SMCodesRenderEngine.cpp" "SMCodesRenderEngine.h"
        HelloTriangleApplication.cpp
        HelloTriangleApplication.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET SMCodesRenderEngine PROPERTY CXX_STANDARD 20)
endif ()

#Find Vulkan
find_package(Vulkan REQUIRED)
# Link Vulkan
target_link_libraries(SMCodesRenderEngine PRIVATE Vulkan::Vulkan)
# Find GLFW package
find_package(glfw3 REQUIRED)
# Link GLFW Library
target_link_libraries(SMCodesRenderEngine PRIVATE glfw)
# Find GLM package
find_package(glm REQUIRED)
# Link GLM Library
target_link_directories(SMCodesRenderEngine PRIVATE glm)

#.\vcpkg install imgui[glfw-binding,core,vulkan-binding]:x64-windows --recurse
# Find imgui
find_package(imgui CONFIG REQUIRED)
# Link imgui
target_link_libraries(SMCodesRenderEngine PRIVATE imgui::imgui)


