#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "dc_utils.h"

struct Semaphore
{
    VkSemaphore handle;
};

typedef struct I_Semaphore
{
    struct Semaphore* (*init)(struct Device* device);
    void (*destroy)(struct Semaphore* semaphore, struct Device* device);
} I_Semaphore;

const I_Semaphore* Semaphore(void);

#endif