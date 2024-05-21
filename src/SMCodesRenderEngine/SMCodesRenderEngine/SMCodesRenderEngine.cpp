// SMCodesRenderEngine.cpp : Defines the entry point for the application.
//
#include "SMCodesRenderEngine.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "HelloTriangleApplication.h"

using namespace std;

int main() {
    HelloTriangleApplication app = HelloTriangleApplication();
    
    try{
       app.run(); 
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
