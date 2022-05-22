#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "Surface.h"

typedef struct PhysicalDevice
{
    VkPhysicalDevice handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceFeatures features;
    u32 graphics_queue_index;
    u32 present_queue_index;
} PhysicalDevice;

PhysicalDevice* physical_device_init(Instance* instance, Surface* surface);
void physical_device_free(PhysicalDevice* physical_device);

#endif