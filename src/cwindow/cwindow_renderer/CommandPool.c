#include "cwindow/cwindow_renderer/CommandPool.h"

static struct CommandPool* init(struct Device* device, struct PhysicalDevice* physical_device, u32 framebuffers_count)
{
    struct CommandPool* command_pool = calloc(1, sizeof(struct CommandPool));

    VkCommandPoolCreateInfo command_pool_ci = { 0 };
    command_pool_ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_ci.queueFamilyIndex = physical_device->graphics_queue_index;

    VK_CHECK(vkCreateCommandPool(device->handle, &command_pool_ci, NULL, &command_pool->handle));

    command_pool->buffers_count = framebuffers_count;
    command_pool->buffers = malloc(sizeof(VkCommandBuffer) * command_pool->buffers_count);
    
    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = command_pool->handle;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = command_pool->buffers_count;

    VK_CHECK(vkAllocateCommandBuffers(device->handle, &command_buffer_allocate_info, command_pool->buffers));

    return command_pool;
}

static void destroy(struct CommandPool* command_pool, struct Device* device)
{
    vkFreeCommandBuffers(device->handle, command_pool->handle, command_pool->buffers_count, command_pool->buffers);
    vkDestroyCommandPool(device->handle, command_pool->handle, NULL);
    free(command_pool->buffers);
    free(command_pool);
}

static I_CommandPool I_COMMAND_POOL = {
    .init = init,
    .destroy = destroy,
};

const I_CommandPool* CommandPool(void)
{
    return &I_COMMAND_POOL;
}