#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "PhysicalDevice.h"

typedef struct CommandPool
{
    VkCommandPool handle;
    VkCommandBuffer* buffers;
    u32 buffers_count;
} CommandPool;

CommandPool* command_pool_init(Device* device, PhysicalDevice* physical_device, u32 max_frames_in_flight);
void command_pool_free(CommandPool* command_pool, Device* device);

#endif