#ifndef INCLUDE_VULKANINTERFACE_H_
#define INCLUDE_VULKANINTERFACE_H_

#include <vulkan/vulkan.h>

struct VulkanQueueFamilyIndicies {
private:
    struct VulkanIndex {
        uint32_t idx, valid;
    };
public:
    // Note that when adding new queue families, the correponding check in
    // fill_queue_family_idxs() in initialization.h has to be added.
    VulkanIndex graphics;
    bool all_found() { return graphics.valid; }
};

struct VulkanInterface {
    static VkInstance                instance;
    static VkDebugUtilsMessengerEXT  debug_messenger;
    static VkPhysicalDevice          physical_device;
    static VulkanQueueFamilyIndicies q_family_idx;
};

#endif // INCLUDE_VULKANINTERFACE_H_