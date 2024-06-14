# CPP Renderer Setup

## Installations
1. CMake - https://cmake.org/download/
2. Visual Studio - Basically have everything installed so that you can make a blank CMake Project
3. CLion - https://www.jetbrains.com/clion/
4. Download Source Code
5. In Clion Do Not Use MinGW as the tool Chain, use visual studio 
   1. ![ProjectWizardCLion.png](ProjectWizardCLion.png)
   2. ![ProjectWizardClion2.png](ProjectWizardClion2.png)
   3. Enable x64-debug and x64-release Profiles ![ProjectWizardClion3.png](ProjectWizardClion3.png)
6. Download vcpkg - https://vcpkg.io/en/ & https://github.com/microsoft/vcpkg/releases/
7. After bootstrapping vcpkg run $vcpkg integrate install
8. Note down the -DCMAKE_TOOLCHAIN_FILE location for example "-DCMAKE_TOOLCHAIN_FILE=C:/Users/shane/Documents/repos/vcpkg/scripts/buildsystems/vcpkg.cmake"
9. Install glfw and glew using ./vcpkg.exe install glfw3:x86-windows glfw3:x64-windows glew:x86-windows glew:x64-windows
10. Install glm using ./vcpkg.exe install glm
11. Download Vulkan from https://vulkan.lunarg.com/sdk/home#windows and install 
12. Install vulkan using vcpkg $.\vcpkg install vulkan
13. Add DCMAKE to CMake Options 
    1. ![CLionCMakeOptions.png](CLionCMakeOptions.png)
14. Update the CMake Presets for the other profiles to point to vcpkg
    1. ![CLionCmakePresetsJson.png](CLionCmakePresetsJson.png)
15. Lastly fix the shader compiler location for the vulkan sdk ![shader_compiler.bat.png](shader_compiler.bat.png)