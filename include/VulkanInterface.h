#ifndef INCLUDE_VULKANINTERFACE_H_
#define INCLUDE_VULKANINTERFACE_H_

#include <vulkan/vulkan.h>

struct VulkanQueueFamilyIndicies {
private:
    struct VulkanIndex {
        uint32_t idx, valid;
    };
public:
    VulkanIndex graphics;
};

struct VulkanInterface {
    static VkInstance instance;
    static VkDebugUtilsMessengerEXT debug_messenger;
    static VkPhysicalDevice physical_device;
    static VulkanQueueFamilyIndicies q_family_idx;
};

#endif // INCLUDE_VULKANINTERFACE_H_