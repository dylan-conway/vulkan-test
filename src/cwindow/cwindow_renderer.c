#include "cwindow/cwindow_renderer.h"
#include "cwindow/cwindow_renderer/Instance.h"
#include "cwindow/cwindow_renderer/Surface.h"
#include "cwindow/cwindow_renderer/PhysicalDevice.h"
#include "cwindow/cwindow_renderer/Device.h"
#include "cwindow/cwindow_renderer/Swapchain.h"
#include "cwindow/cwindow_renderer/Pipeline.h"
#include "cwindow/cwindow_renderer/CommandPool.h"
#include "cwindow/cwindow_renderer/Fence.h"
#include "cwindow/cwindow_renderer/Semaphore.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#define METHOD(t, m) t##____##m

void METHOD(cwindow_renderer, present)(void);
void METHOD(cwindow_renderer, clear)(void);

static struct Instance* instance = NULL;
static struct Surface* surface = NULL;
static struct PhysicalDevice* physical_device = NULL;
static struct Device* device = NULL;
static struct Swapchain* swapchain = NULL;
static struct Pipeline* pipeline = NULL;
static struct CommandPool* command_pool = NULL;
static struct Fence** in_flight_fences = NULL;
static struct Semaphore** image_available_semaphores = NULL;
static struct Semaphore** render_finished_semaphores = NULL;

static u32 current_frame = 0;
static u32 max_frames_in_flight = 2;

cwindow_renderer cwindow_renderer_init(void* sdl_window)
{
    instance = Instance()->init(sdl_window, "3d cubes", "cwindow");
    Instance()->setup_debug_utils(instance);
    
    surface = Surface()->init(instance, sdl_window);
    physical_device = PhysicalDevice()->init(instance, surface);
    device = Device()->init(physical_device);
    swapchain = Swapchain()->init(device, physical_device, surface);
    pipeline = Pipeline()->init(device, swapchain);
    command_pool = CommandPool()->init(device, physical_device, max_frames_in_flight);

    image_available_semaphores = malloc(sizeof(struct Semaphore*) * max_frames_in_flight);
    render_finished_semaphores = malloc(sizeof(struct Semaphore*) * max_frames_in_flight);
    in_flight_fences = malloc(sizeof(struct Fence*) * max_frames_in_flight);

    for (u32 i = 0; i < max_frames_in_flight; i ++)
    {
        image_available_semaphores[i] = Semaphore()->init(device);
        render_finished_semaphores[i] = Semaphore()->init(device);
        in_flight_fences[i] = Fence()->init(device);
    }

    cwindow_renderer renderer = {
        .clear = METHOD(cwindow_renderer, clear),
        .present = METHOD(cwindow_renderer, present),
    };

    return renderer;
}

void cwindow_renderer_free(void)
{
    vkQueueWaitIdle(device->present_queue);
    for (u32 i = 0; i < max_frames_in_flight; i ++)
    {

        Semaphore()->destroy(image_available_semaphores[i], device);
        Semaphore()->destroy(render_finished_semaphores[i], device);
        Fence()->destroy(in_flight_fences[i], device);
    }
    free(image_available_semaphores);
    free(render_finished_semaphores);
    free(in_flight_fences);
    CommandPool()->destroy(command_pool, device);
    Pipeline()->destroy(pipeline, device);
    Swapchain()->destroy(swapchain, device);
    Device()->destroy(device);
    PhysicalDevice()->destroy(physical_device);
    Surface()->destroy(surface, instance);
    Instance()->destroy(instance);
}

void cwindow_renderer_handle_resize(void)
{

}

void METHOD(cwindow_renderer, clear)(void)
{

}

void METHOD(cwindow_renderer, present)(void)
{
    Fence()->wait(in_flight_fences[current_frame], device);
    Fence()->reset(in_flight_fences[current_frame], device);

    u32 image_index;
    vkAcquireNextImageKHR(device->handle, swapchain->handle, UINT64_MAX, image_available_semaphores[current_frame]->handle, VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(command_pool->buffers[current_frame], 0);
    
    { // Record command buffer
        VkCommandBuffer command_buffer = command_pool->buffers[current_frame];

        VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // TODO ("silent check")
        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

        VkClearValue clear_value = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};

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

        // TODO ("silent check")
        vkEndCommandBuffer(command_buffer);
    }

    VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    };

    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &image_available_semaphores[current_frame]->handle;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_pool->buffers[current_frame];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &render_finished_semaphores[current_frame]->handle;

    vkQueueSubmit(device->graphics_queue, 1, &submit_info, in_flight_fences[current_frame]->handle);

    VkPresentInfoKHR present_info = { 0 };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_finished_semaphores[current_frame]->handle;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &image_index;

    vkQueuePresentKHR(device->present_queue, &present_info);

    current_frame = (current_frame + 1) % max_frames_in_flight;
}