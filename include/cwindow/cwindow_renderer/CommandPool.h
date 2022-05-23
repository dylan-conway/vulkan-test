#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "PhysicalDevice.h"

struct CommandPool
{
    VkCommandPool handle;
    VkCommandBuffer* buffers;
    u32 buffers_count;
};

typedef struct I_CommandPool
{
    struct CommandPool* (*init)(struct Device* device, struct PhysicalDevice* physical_device, u32 framebuffers_count);
    void (*destroy)(struct CommandPool* command_pool, struct Device* device);
} I_CommandPool;

const I_CommandPool* CommandPool(void);

#endif