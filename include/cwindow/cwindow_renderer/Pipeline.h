#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "Swapchain.h"

typedef struct Pipeline
{
    VkPipeline handle;
    VkPipelineLayout layout;
    VkRenderPass render_pass;
    VkFramebuffer* framebuffers;
    u32 framebuffers_count;
} Pipeline;

Pipeline* pipeline_init(Device* device, Swapchain* swapchain);
void pipeline_free(Pipeline* pipeline, Device* device);

#endif