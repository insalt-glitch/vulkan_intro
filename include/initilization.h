#ifndef INCLUDE_INITIALIZATION_H_
#define INCLUDE_INITIALIZATION_H_

#include <stdio.h>
#include <cstring>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "VulkanInterface.h"
#include "constants.h"

void init_window(GLFWwindow** wnd, int width, int height);

void init_vulkan();

#endif  // INCLUDE_INITIALIZATION_H_