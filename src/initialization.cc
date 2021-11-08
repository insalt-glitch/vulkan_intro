static void check_vulkan_extension_support(uint32_t num_req_ext,
                                           const char** req_ext,
                                           bool print_info = PRINT_ADDITIONAL_INFO) {
    uint32_t num_extensions = 0, i, j;
    vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, nullptr);
    VkExtensionProperties* extensions = new VkExtensionProperties[num_extensions];
    vkEnumerateInstanceExtensionProperties(nullptr, &num_extensions, extensions);
    for (i = 0; i < num_req_ext; ++i) {
        j = num_extensions;
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
        i = num_extensions;
        fprintf(stdout, "(Vulkan) Available vulkan extensions\n(Vulkan)\n");
        while (i--) {
            fprintf(stdout, "(Vulkan) \t%s\n", extensions[i].extensionName);
        }
        fprintf(stdout, "(Vulkan) \n");
    }
    delete[] extensions;
}

static bool fill_queue_family_idxs(VulkanQueueFamilyIndicies& q_indicies,
                                   VkPhysicalDevice device) {
    uint32_t num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,
                                             &num_queue_families,
                                             nullptr);

    VkQueueFamilyProperties* queue_families =
            new VkQueueFamilyProperties[num_queue_families];
    vkGetPhysicalDeviceQueueFamilyProperties(device,
                                             &num_queue_families, queue_families);
    for (uint32_t i = 0; i < num_queue_families; ++i) {
        // checks for different queues
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            q_indicies.graphics.idx = i;
            q_indicies.graphics.valid  = true;
        }
    }
    return q_indicies.all_found();
}

static bool check_vulkan_suitable_device(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VulkanQueueFamilyIndicies q_indicies;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);
    bool is_suitable = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
                       features.geometryShader &&
                       fill_queue_family_idxs(q_indicies, device);
    if (is_suitable)
        VulkanInterface::q_family_idx = q_indicies;
    return is_suitable;
}

static const char** get_vulkan_required_extensions(uint32_t* num_required_extensions) {
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

static void create_vulkan_instance() {
    // set up creation of the vulkan instance
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
    req_extensions = get_vulkan_required_extensions(&num_req_extension);
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

    if (vkCreateInstance(&create_info, nullptr, &VulkanInterface::instance) != VK_SUCCESS)
        throw std::runtime_error("\nFailed to create vulkan instance.\n");
}

static void pick_vulkan_physical_device(bool print_info = PRINT_ADDITIONAL_INFO) {
    uint32_t num_devices = 0, i;
    vkEnumeratePhysicalDevices(VulkanInterface::instance, &num_devices, nullptr);
    if (!num_devices)
        throw std::runtime_error("No GPUs with vulkan support found.");
    VkPhysicalDevice* devices = new VkPhysicalDevice[num_devices];
    vkEnumeratePhysicalDevices(VulkanInterface::instance, &num_devices, devices);
    // we currenty only use a single device
    i = num_devices;
    while(check_vulkan_suitable_device(devices[--i]) == false) {
        if (i == 0)
            throw std::runtime_error("No suitable GPUs for this application.");
    }
    VulkanInterface::physical_device = devices[i];

    if (print_info) {
        VkPhysicalDeviceProperties properties;
        fprintf(stdout, "(Vulkan) Available device(s)\n(Vulkan)\n");
        for (i = 0; i < num_devices; ++i) {
            vkGetPhysicalDeviceProperties(devices[i], &properties);
            fprintf(stdout, "(Vulkan) \t%s\n", properties.deviceName);
        }
        fprintf(stdout, "(Vulkan)\n");
    }

    delete[] devices;
}

static void init_window(GLFWwindow** wnd, int width, int height) {
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

static void init_vulkan() {
    create_vulkan_instance();
    create_debug_messenger();
    pick_vulkan_physical_device();
}
