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

static void reset_buffer(struct CommandPool* command_pool, u32 frame_index)
{
    vkResetCommandBuffer(command_pool->buffers[frame_index], 0);
}

static VkClearValue clear_value = { 0 };

static void record_buffer(struct CommandPool* command_pool, u32 frame_index, u32 image_index, struct Pipeline* pipeline, struct Swapchain* swapchain)
{
    VkCommandBuffer command_buffer = command_pool->buffers[frame_index];

    VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

    VkRenderPassBeginInfo render_pass_begin_info = { 0 };
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = pipeline->render_pass;
    render_pass_begin_info.framebuffer = pipeline->framebuffers[image_index];
    render_pass_begin_info.renderArea.extent = swapchain->image_extent;
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_value;

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle);

    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    vkEndCommandBuffer(command_buffer);
}

static void set_clear_color(struct CommandPool* command_pool, color c)
{
    clear_value.color.float32[0] = c.r / 255.0f;
    clear_value.color.float32[1] = c.g / 255.0f;
    clear_value.color.float32[2] = c.b / 255.0f;
    clear_value.color.float32[3] = c.a / 255.0f;
}

static I_CommandPool I_COMMAND_POOL = {
    .init = init,
    .destroy = destroy,
    .reset_buffer = reset_buffer,
    .record_buffer = record_buffer,
    .set_clear_color = set_clear_color,
};

const I_CommandPool* CommandPool(void)
{
    return &I_COMMAND_POOL;
}