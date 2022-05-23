#include "cwindow/cwindow_renderer/Fence.h"

static struct Fence* init(struct Device* device)
{
    struct Fence* fence = calloc(1, sizeof(struct Fence));

    VkFenceCreateInfo fence_ci = { 0 };
    fence_ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_ci.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CHECK(vkCreateFence(device->handle, &fence_ci, NULL, &fence->handle));

    return fence;
}

static void destroy(struct Fence* fence, struct Device* device)
{
    vkDestroyFence(device->handle, fence->handle, NULL);
    free(fence);
}

static void reset(struct Fence* fence, struct Device* device)
{
    vkResetFences(device->handle, 1, &fence->handle);
}

static void wait(struct Fence* fence, struct Device* device)
{
    vkWaitForFences(device->handle, 1, &fence->handle, VK_TRUE, UINT64_MAX);
}

static I_Fence I_FENCE = {
    .init = init,
    .destroy = destroy,
    .reset = reset,
    .wait = wait,
};

const I_Fence* Fence(void)
{
    return &I_FENCE;
}