#include <stdio.h>
#include <stdexcept>

#include "../include/initilization.h"
#include "../include/main_loop.h"
#include "../include/cleanup.h"

// init vulkan interface
VkInstance VulkanInterface::instance                      = nullptr;
VkDebugUtilsMessengerEXT VulkanInterface::debug_messenger = nullptr;
VkPhysicalDevice VulkanInterface::physical_device         = nullptr;
VulkanQueueFamilyIndicies VulkanInterface::q_family_idx   = {0,0};

int main(/* int argc, char* argv[] */) {
    GLFWwindow* window = nullptr;
    try {
        // initilization
        init_window(&window, WND_WIDTH, WND_HEIGHT);
        fprintf(stdout, "Created window.\n");
        init_vulkan();
        fprintf(stdout, "Initialized vulkan.\n");
        // run task
        main_loop(window);
        // destroy objects and clean memory
        cleanup(window);
        fprintf(stdout, "Cleanup objects.\n");
    } catch (std::exception& e) {
        fprintf(stderr, "Execution failed. Runtime-Error: %s\n", e.what());
        return 1;
    }
    return 0;
}