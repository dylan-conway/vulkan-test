#ifndef FENCE_H
#define FENCE_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "dc_utils.h"

struct Fence
{
    VkFence handle;
};

typedef struct I_Fence
{
    struct Fence* (*init)(struct Device* device);
    void (*destroy)(struct Fence* fence, struct Device* device);
    void (*reset)(struct Fence* fence, struct Device* device);
    void (*wait)(struct Fence* fence, struct Device* device);
} I_Fence;

const I_Fence* Fence(void);

#endif