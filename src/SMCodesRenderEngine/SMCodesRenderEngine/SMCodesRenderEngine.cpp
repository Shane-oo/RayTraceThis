// SMCodesRenderEngine.cpp : Defines the entry point for the application.
//
#include "SMCodesRenderEngine.h"

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
