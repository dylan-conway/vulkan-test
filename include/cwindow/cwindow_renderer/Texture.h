#ifndef TEXTURE_H
#define TEXTURE_H

#include <vulkan/vulkan.h>
#include "Device.h"

struct Texture
{
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;
};

typedef struct I_Texture
{
    struct Texture* (*init)(struct Device* device);
    void (*destroy)(struct Texture* texture, struct Device* device);
} I_Texture;

const I_Texture* Texture(void);

#endif