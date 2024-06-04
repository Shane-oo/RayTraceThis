@echo off
setlocal enabledelayedexpansion

:: Define the path to the glslc compiler
set GLSLC=C:\VulkanSDK\1.3.275.0\Bin\glslc.exe

:: Define the shaders folder
set SHADERS_DIR=SMCodesRenderEngine\shaders

:: Loop through all shader files in the shaders folder
for %%f in (%SHADERS_DIR%\*.frag %SHADERS_DIR%\*.vert) do (
    :: Determine the shader stage based on the file extension
    set STAGE=
    if "%%~xf"==".vert" set STAGE=vertex
    if "%%~xf"==".frag" set STAGE=fragment

    if defined STAGE (
        echo Compiling %%f as !STAGE! shader
        %GLSLC% -fshader-stage=!STAGE! %%f -o %%f.spv
    ) else (
        echo Skipping %%f: Unknown shader stage
    )
)