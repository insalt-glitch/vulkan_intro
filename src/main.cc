#include <stdio.h>
#include <stdexcept>

#include "../include/initilization.h"
#include "../include/main_loop.h"
#include "../include/cleanup.h"

int main(/* int argc, char* argv[] */) {
    GLFWwindow* window = nullptr;
    VkInstance vk_instance = nullptr;
    VkDebugUtilsMessengerEXT vk_debug_messenger;
    VkPhysicalDevice physical_device;
    try {
        // initilization
        init_window(&window, WND_WIDTH, WND_HEIGHT);
        fprintf(stdout, "Created window.\n");
        init_vulkan(&vk_instance, &vk_debug_messenger, &physical_device);
        fprintf(stdout, "Initialized vulkan.\n");
        // run task
        main_loop(window);
        // destroy objects and clean memory
        cleanup(window, vk_instance, vk_debug_messenger);
        fprintf(stdout, "Cleanup objects.\n");
    } catch (std::exception& e) {
        fprintf(stderr, "Execution failed. Runtime-Error: %s\n", e.what());
        return 1;
    }
    return 0;
}