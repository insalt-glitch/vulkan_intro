#include "../include/initilization.h"

#include <stdio.h>
#include <cstring>
#include <stdexcept>

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
    // Message is important enough to show
    fprintf(stdout, "(Vulkan) Debug Message:\t");
    uint32_t i = 0;
    while(pCallbackData->pMessage[i] != 0) {
        fprintf(stdout, "%c", pCallbackData->pMessage[i]);
        if (!(++i % (86))) {
            fprintf(stdout, "\n(Vulkan) \t\t");
        }
    }
    fprintf(stderr, "\n(Vulkan) \t Flags: ");
    if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        fprintf(stdout, "(SEVERE) ");
    }
    if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
        fprintf(stdout, "(VALIDATION/VIOLATION) ");
    }
    if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
        fprintf(stdout, "(PERFORMANCE) ");
    }
    fprintf(stdout, "\n");
    pUserData = nullptr;
    return VK_FALSE;
}

static void populate_debug_messenger_create_info(
        VkDebugUtilsMessengerCreateInfoEXT* create_info) {
    create_info->sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info->messageSeverity = //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info->messageType     = //VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info->pfnUserCallback = debug_callback;
}

static void check_vulkan_extension_support(uint32_t num_req_ext,
                                           const char** req_ext,
                                           bool print_info = false) {
    uint32_t num_extensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, nullptr);
    VkExtensionProperties* extensions = new VkExtensionProperties[num_extensions];
    vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, extensions);
    for (uint32_t i = 0; i < num_req_ext; ++i) {
        uint32_t j = num_extensions;
        while (strcmp(req_ext[i], extensions[--j].extensionName)) {
            if (j == 0) {
                fprintf(stderr, "(Vulkan) Required vulkan extension \"%s\""
                        " was not found!\n",
                        req_ext[i]);
                throw std::runtime_error("(Vulkan) Missing extensions\n");
            }
        }
    }
    // optionally print available vulkan extensions
    if (print_info) {
        fprintf(stdout, "(Vulkan) Available vulkan extensions\n\n");
        while (num_extensions--) {
            fprintf(stdout, "(Vulkan) \t%s\n", extensions[num_extensions].extensionName);
        }
        fprintf(stdout, "(Vulkan) \n");
    }
    delete[] extensions;
}

static void check_vulkan_validation_layer_support(uint32_t num_req_layers,
                                        const char* req_layers[]) {
    uint32_t num_layers, j;
    vkEnumerateInstanceLayerProperties(&num_layers, nullptr);
    VkLayerProperties* layers = new VkLayerProperties[num_layers];
    vkEnumerateInstanceLayerProperties(&num_layers, layers);

    for (uint32_t i = 0; i < num_req_layers; ++i) {
        j = num_layers - 1;
        while (strcmp(req_layers[i], layers[--j].layerName)) {
            if (j == 0) {
                fprintf(stderr, "Required vulkan validation layer \"%s\""
                        " was not found!\n", req_layers[i]);
                throw std::runtime_error("Missing validation layer.\n");
            }
        }
    }
    delete[] layers;
}

static const char** vulkan_required_extensions(uint32_t* num_required_extensions) {
    uint32_t num_extensions = 0;
    const char** extensions;
    extensions = glfwGetRequiredInstanceExtensions(&num_extensions);
    if (VULKAN_VALIDATION_LAYER) {
        num_extensions += 1;
        const char** req_extensions = new const char*[num_extensions];
        for (uint32_t i = 0; i < num_extensions - 1; ++i) {
            req_extensions[i] = extensions[i];
        }
        req_extensions[num_extensions - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        extensions = req_extensions;
    }
    *num_required_extensions = num_extensions;
    return extensions;
}

static VkInstance create_vulkan_instance() {
    // set up creation of a vulkan instance
    VkInstance instance = nullptr;
    VkApplicationInfo app_info{};
    VkInstanceCreateInfo create_info{};
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    uint32_t num_req_extension = 0;
    const char** req_extensions = nullptr;

    // fill app-info struct
    app_info.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName    = "intro to vulkan";
    app_info.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName         = "No Engine";
    app_info.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion          = VK_API_VERSION_1_0;

    // fill create-info struct
    req_extensions = vulkan_required_extensions(&num_req_extension);
    create_info.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo         = &app_info;
    create_info.enabledExtensionCount    = num_req_extension;
    create_info.ppEnabledExtensionNames  = req_extensions;

    // check support for extensions and validation layers
    check_vulkan_extension_support(num_req_extension, req_extensions);
    if (VULKAN_VALIDATION_LAYER) {
        const char* required_layers[] = {"VK_LAYER_KHRONOS_validation"};
        const uint32_t num_required_layers = sizeof(required_layers) / sizeof(const char*);
        check_vulkan_validation_layer_support(num_required_layers, required_layers);
        create_info.enabledLayerCount    = num_required_layers;
        create_info.ppEnabledLayerNames  = required_layers;
        populate_debug_messenger_create_info(&debug_create_info);
        create_info.pNext = &debug_create_info;
    } else {
        create_info.enabledLayerCount    = 0;
        create_info.ppEnabledLayerNames  = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("\nFailed to create vulkan instance.\n");
    return instance;
}

static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static VkDebugUtilsMessengerEXT init_debug_messenger(VkInstance* instance) {
    if (VULKAN_VALIDATION_LAYER) {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        VkDebugUtilsMessengerEXT debug_messenger;
        populate_debug_messenger_create_info(&create_info);

        if (CreateDebugUtilsMessengerEXT(*instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
            throw std::runtime_error("(Vulkan) Failed to set up debug messenger!");
        return debug_messenger;
    }
    return nullptr;
}

void init_window(GLFWwindow** wnd, int width, int height) {
    glfwInit();
    // no OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // no resizeable window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // create window and move it
    *wnd = glfwCreateWindow(width, height, "Intro to Vulkan", nullptr, nullptr);
    int top;
    glfwGetWindowFrameSize(*wnd, nullptr, &top, nullptr, nullptr);
    glfwSetWindowPos(*wnd, 0, top);
}

void init_vulkan(VkInstance* instance, VkDebugUtilsMessengerEXT* debug_messenger) {
    *instance = create_vulkan_instance();
    *debug_messenger = init_debug_messenger(instance);
}
