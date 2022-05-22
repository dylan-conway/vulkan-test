#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan/vulkan.h>

typedef struct Buffer
{
    VkBuffer handle;
    VkDeviceMemory memory;
} Buffer;

Buffer* buffer_init();
void buffer_free(Buffer* buffer);

#endif