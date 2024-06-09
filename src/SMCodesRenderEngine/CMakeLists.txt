# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# run commands
if(WIN32)
  execute_process(COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cmd.exe /c "shader_compiler.bat"
          RESULT_VARIABLE result
          OUTPUT_VARIABLE output
          ERROR_VARIABLE error_output
  )
  if(result EQUAL 0)
    message(STATUS "Shaders Compiled Successfully")
  else()
    message(FATAL_ERROR "Error executing Shader Compiler: ${error_output}")
  endif()
endif()


project ("SMCodesRenderEngine")

# Include sub-projects.
add_subdirectory ("SMCodesRenderEngine")


###