#ifndef INITIALIZATION_H_
#define INITIALIZATION_H_

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vulkan/vulkan.h>

static void checkExtensionSupport(const char** req_ext,
                                  uint32_t num_req_ext,
                                  bool print_info=false) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr,
                                           &extensionCount,
                                           nullptr);
    VkExtensionProperties* extensions =
            new VkExtensionProperties[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr,
                                           &extensionCount,
                                           extensions);
    for (size_t i = 0; i < num_req_ext; ++i) {
        size_t j = extensionCount - 1;
        while (req_ext[i] == extensions[j].extensionName) {
            if (--j < 0) {
                fprintf(stderr, "Required vulkan extension \"%s\""
                        "was not found!\n",
                        req_ext[i]);
                throw 1;
            }
        }
    }
    // optionally print available vulkan extensions
    if (print_info) {
        fprintf(stdout, "Available vulkan extensions\n\n");
        while (extensionCount--) {
            fprintf(stdout, "\t%s\n", extensions[extensionCount].extensionName);
        }
        fprintf(stdout, "\n");
    }
    delete[] extensions;
}

GLFWwindow* init_window(GLFWwindow* wnd, int width, int height) {
    glfwInit();
    // no OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // no resizeable window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // create window and move it
    wnd = glfwCreateWindow(width, height, "Intro to Vulkan", nullptr, nullptr);
    int top;
    glfwGetWindowFrameSize(wnd, nullptr, &top, nullptr, nullptr);
    glfwSetWindowPos(wnd, 0, top);
    return wnd;
}

void init_vulkan() {
    // set up creation of a vulkan instance
    VkInstance instance = nullptr;
    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo createInfo{};
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    // fill app-info struct
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "intro to vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    // fill create-info struct
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    // check extension support
    checkExtensionSupport(glfwExtensions, glfwExtensionCount);

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("\nfailed to create vulkan instance.\n");
}

#endif  // INITIALIZATION_H_