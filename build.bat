@echo off
setlocal

:: Include paths to libs (glm, glfw, vulkan)
set glm_incl=libs/glm
set glfw_incl=libs/glfw/include
set vulkan_incl=libs/VulkanSDK/1.2.189.2/Include
set INCLUDE_LIBS=-I%glm_incl% -I%vulkan_incl% -I%glfw_incl%
:: Link paths to libs (glfw and vulkan)
set glfw_link=libs/glfw/lib-vc2019
set vulkan_link=libs/VulkanSDK/1.2.189.2/Lib
set LINK_LIBS=-L%vulkan_link% -L%glfw_link% -lvulkan-1 -lglfw3dll

:: Set compiler and linker options
set COMPILE_FLAGS=-Wall -Wpedantic -Wextra -Wbad-function-cast -Wcast-align^
                  -Wunreachable-code -Wconversion -std=c++17 %INCLUDE_LIBS%
set LINK_FLAGS=%LINK_LIBS%

:: Build the executable
call clang++ -O0 %COMPILE_FLAGS% %LINK_FLAGS% main.cc src/*.cc -o vulkan_intro.exe
