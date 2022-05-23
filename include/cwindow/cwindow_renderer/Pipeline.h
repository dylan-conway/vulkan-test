#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "Swapchain.h"

struct Pipeline
{
    VkPipeline handle;
    VkPipelineLayout layout;
    VkRenderPass render_pass;
    VkFramebuffer* framebuffers;
    u32 framebuffers_count;
};

typedef struct I_Pipeline
{
    struct Pipeline* (*init)(struct Device* device, struct Swapchain* swapchain);
    void (*destroy)(struct Pipeline* pipeline, struct Device* device);
} I_Pipeline;

const I_Pipeline* Pipeline(void);

#endif