#ifndef DEVICE_H
#define DEVICE_H

#include <vulkan/vulkan.h>
#include "cwindow/cwindow_renderer/Instance.h"
#include "cwindow/cwindow_renderer/PhysicalDevice.h"
#include "dvec.h"

struct Device
{
    VkDevice handle;
    VkQueue graphics_queue;
    VkQueue present_queue;
    dvec_str extensions;
};

typedef struct I_Device
{
    struct Device* (*init)(struct PhysicalDevice* physical_device);
    void (*destroy)(struct Device* device);
    void (*wait)(struct Device* device);
} I_Device;

const I_Device* Device(void);

#endif