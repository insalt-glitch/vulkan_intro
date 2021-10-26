#ifndef INCLUDE_CLEANUP_H_
#define INCLUDE_CLEANUP_H_

#include <GLFW/glfw3.h>

#include "constants.h"

static void DestroyDebugUtilsMessengerEXT(
         VkInstance instance,
         VkDebugUtilsMessengerEXT debugMessenger,
         const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) 
                vkGetInstanceProcAddr(instance,
                                      "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void cleanup(GLFWwindow* wnd) {
    if (VULKAN_VALIDATION_LAYER)
        DestroyDebugUtilsMessengerEXT(VulkanInterface::instance, VulkanInterface::debug_messenger, nullptr);
    vkDestroyInstance(VulkanInterface::instance, nullptr);
    glfwDestroyWindow(wnd);
    glfwTerminate();
}

#endif // INCLUDE_CLEANUP_H_