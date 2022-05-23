#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "Surface.h"
#include "cwindow_renderer_base.h"

struct PhysicalDevice
{
    VkPhysicalDevice handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceFeatures features;
    u32 graphics_queue_index;
    u32 present_queue_index;
};

typedef struct I_PhysicalDevice
{
    struct PhysicalDevice* (*init)(struct Instance* instance, struct Surface* surface);
    void (*destroy)(struct PhysicalDevice* physical_device);
} I_PhysicalDevice;

const I_PhysicalDevice* PhysicalDevice(void);

#endif