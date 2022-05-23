#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan/vulkan.h>
#include "Device.h"

struct Buffer
{
    VkBuffer handle;
    VkDeviceMemory memory;
    VkDeviceSize size;
};

typedef struct I_Buffer
{
    struct Buffer* (*init)(struct Device* device)
}

Buffer* buffer_init();
void buffer_free(Buffer* buffer);

#endif