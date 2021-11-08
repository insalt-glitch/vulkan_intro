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
    } else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
        fprintf(stdout, "(PERFORMANCE) ");
    }
    fprintf(stdout, "\n");
    pUserData = nullptr;
    return VK_FALSE;
}

static void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* create_info) {
    create_info->sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info->messageSeverity = //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info->messageType     = //VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info->pfnUserCallback = debug_callback;
}

static void create_debug_messenger() {
    if (VULKAN_VALIDATION_LAYER) {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        populate_debug_messenger_create_info(&create_info);
        // create debug messenger
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(VulkanInterface::instance, "vkCreateDebugUtilsMessengerEXT");
        if (func == nullptr)
            throw std::runtime_error("Failed to set up debug messenger!");
        func(VulkanInterface::instance, &create_info, nullptr, &VulkanInterface::debug_messenger);
    }
}