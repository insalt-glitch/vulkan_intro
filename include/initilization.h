#ifndef INITIALIZATION_H_
#define INITIALIZATION_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define VULKAN_VALIDATION_LAYER true

void init_window(GLFWwindow** wnd, int width, int height);

void init_vulkan(VkInstance* instance, VkDebugUtilsMessengerEXT* debug_manager);

#endif  // INITIALIZATION_H_