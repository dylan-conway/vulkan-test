#include "cwindow/cwindow_renderer/CommandPool.h"

CommandPool* command_pool_init(Device* device, PhysicalDevice* physical_device, u32 max_frames_in_flight)
{
    CommandPool* command_pool = calloc(1, sizeof(CommandPool));

    VkCommandPoolCreateInfo command_pool_ci = { 0 };
    command_pool_ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_ci.queueFamilyIndex = physical_device->graphics_queue_index;

    VK_CHECK(vkCreateCommandPool(device->handle, &command_pool_ci, NULL, &command_pool->handle));

    command_pool->buffers_count = max_frames_in_flight;
    command_pool->buffers = malloc(sizeof(VkCommandBuffer) * command_pool->buffers_count);
    
    VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = command_pool->handle;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = command_pool->buffers_count;

    VK_CHECK(vkAllocateCommandBuffers(device->handle, &command_buffer_allocate_info, command_pool->buffers));

    return command_pool;
}

void command_pool_free(CommandPool* command_pool, Device* device)
{
    vkFreeCommandBuffers(device->handle, command_pool->handle, command_pool->buffers_count, command_pool->buffers);
    free(command_pool->buffers);
    free(command_pool);
}
