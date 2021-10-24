#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdexcept>

#include "initilization.h"
#include "main_loop.h"
#include "cleanup.h"

#define WND_WIDTH 800
#define WND_HEIGHT 600

int main(int argc, char* argv[]) {
    GLFWwindow* window = nullptr;

    // initilization
    window = init_window(window, WND_WIDTH, WND_HEIGHT);
    init_vulkan();
    fprintf(stdout, "Successful initialization! Starting application...\n\n");
    main_loop(window);
    cleanup(window);
    
    return 0;
}