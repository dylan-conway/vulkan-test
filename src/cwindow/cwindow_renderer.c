#include "cwindow/cwindow_renderer.h"
#include "cwindow/cwindow_renderer/Instance.h"
#include "cwindow/cwindow_renderer/Surface.h"
#include "cwindow/cwindow_renderer/PhysicalDevice.h"
#include "cwindow/cwindow_renderer/Device.h"
#include "cwindow/cwindow_renderer/Swapchain.h"
#include "cwindow/cwindow_renderer/Pipeline.h"
#include "cwindow/cwindow_renderer/Shader.h"
#include "cwindow/cwindow_renderer/CommandPool.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#define METHOD(t, m) t##____##m

void METHOD(cwindow_renderer, present)(void);
void METHOD(cwindow_renderer, clear)(void);

static Instance* instance = NULL;
static Surface* surface = NULL;
static PhysicalDevice* physical_device = NULL;
static Device* device = NULL;
static Swapchain* swapchain = NULL;
static Pipeline* pipeline = NULL;
static CommandPool* command_pool = NULL;

// TODO
static u32 current_frame = 0;
static u32 max_frames_in_flight = 2;
static VkSemaphore* image_available_semaphores = NULL;
static VkSemaphore* render_finished_semaphores = NULL;
static VkFence* in_flight_fences = NULL;

// static 

cwindow_renderer cwindow_renderer_init(void* sdl_window)
{
    instance = instance_init(sdl_window, "3d cubes", "cwindow");
    instance_setup_debug_utils(instance);
    
    surface = surface_init(instance, sdl_window);
    physical_device = physical_device_init(instance, surface);
    device = device_init(instance, physical_device);
    swapchain = swapchain_init(instance, device, physical_device, surface);
    pipeline = pipeline_init(device, swapchain);

    image_available_semaphores = malloc(sizeof(VkSemaphore) * max_frames_in_flight);
    render_finished_semaphores = malloc(sizeof(VkSemaphore) * max_frames_in_flight);
    in_flight_fences = malloc(sizeof(VkFence) * max_frames_in_flight);

    VkSemaphoreCreateInfo semaphore_ci = { 0 };
    semaphore_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_ci = { 0 };
    fence_ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_ci.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (u32 i = 0; i < max_frames_in_flight; i ++)
    {
        VK_CHECK(vkCreateSemaphore(device->handle, &semaphore_ci, NULL, &image_available_semaphores[i]));
        VK_CHECK(vkCreateSemaphore(device->handle, &semaphore_ci, NULL, &render_finished_semaphores[i]));
        VK_CHECK(vkCreateFence(device->handle, &fence_ci, NULL, &in_flight_fences[i]));
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
        vkDestroySemaphore(device->handle, image_available_semaphores[i], NULL);
        vkDestroySemaphore(device->handle, render_finished_semaphores[i], NULL);
        vkDestroyFence(device->handle, in_flight_fences[i], NULL);
    }
    free(image_available_semaphores);
    free(render_finished_semaphores);
    free(in_flight_fences);
    command_pool_free(command_pool, device);
    pipeline_free(pipeline, device);
    swapchain_free(swapchain, device);
    device_free(device, instance);
    physical_device_free(physical_device);
    surface_free(surface, instance);
    instance_free(instance);
}

void cwindow_renderer_handle_resize(void)
{
    // vkQueueWaitIdle(device->present_queue);
    // vkFreeCommandBuffers(device->handle, command_pool, max_frames_in_flight, command_buffers);
    // free(command_buffers);
    // for (u32 i = 0; i < max_frames_in_flight; i ++)
    // {
    //     vkDestroySemaphore(device->handle, image_available_semaphores[i], NULL);
    //     vkDestroySemaphore(device->handle, render_finished_semaphores[i], NULL);
    //     vkDestroyFence(device->handle, in_flight_fences[i], NULL);
    // }
    // free(image_available_semaphores);
    // free(render_finished_semaphores);
    // free(in_flight_fences);
    // vkDestroyCommandPool(device->handle, command_pool, NULL);
    // for (u32 i = 0; i < swapchain->image_count; i ++)
    // {
    //     vkDestroyFramebuffer(device->handle, framebuffers[i], NULL);
    // }
}

void METHOD(cwindow_renderer, clear)(void)
{

}

void METHOD(cwindow_renderer, present)(void)
{
    vkWaitForFences(device->handle, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);
    vkResetFences(device->handle, 1, &in_flight_fences[current_frame]);

    u32 image_index;
    vkAcquireNextImageKHR(device->handle, swapchain->handle, UINT64_MAX, image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

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
    submit_info.pWaitSemaphores = &image_available_semaphores[current_frame];
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_pool->buffers[current_frame];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &render_finished_semaphores[current_frame];

    vkQueueSubmit(device->graphics_queue, 1, &submit_info, in_flight_fences[current_frame]);

    VkPresentInfoKHR present_info = { 0 };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_finished_semaphores[current_frame];
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &image_index;

    vkQueuePresentKHR(device->present_queue, &present_info);

    current_frame = (current_frame + 1) % max_frames_in_flight;
}