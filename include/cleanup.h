#ifndef CLEANUP_H_
#define CLEANUP_H_

#include <GLFW/glfw3.h>

#define VULKAN_VALIDATION_LAYER true

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

void cleanup(GLFWwindow* wnd, VkInstance instance,
             VkDebugUtilsMessengerEXT debug_messenger) {
    if (VULKAN_VALIDATION_LAYER)
        DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(wnd);
    glfwTerminate();
}

#endif // CLEANUP_H_