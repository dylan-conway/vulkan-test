#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include <vulkan/vulkan.h>
#include "cwindow/cwindow_types.h"
#include "Device.h"
#include "PhysicalDevice.h"
#include "Pipeline.h"
#include "Swapchain.h"

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
    void (*reset_buffer)(struct CommandPool* command_pool, u32 frame_index);
    void (*record_buffer)(struct CommandPool* command_pool, u32 frame_index, u32 image_index, struct Pipeline* pipeline, struct Swapchain* swapchain);
    void (*set_clear_color)(struct CommandPool* command_pool, color c);
} I_CommandPool;

const I_CommandPool* CommandPool(void);

#endif