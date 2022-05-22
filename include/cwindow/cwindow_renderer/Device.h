#ifndef DEVICE_H
#define DEVICE_H

#include <vulkan/vulkan.h>
#include "cwindow/cwindow_renderer/Instance.h"
#include "cwindow/cwindow_renderer/PhysicalDevice.h"

typedef struct Device
{
    VkDevice handle;
    VkQueue graphics_queue;
    VkQueue present_queue;
    gvec extensions;
} Device;

Device* device_init(Instance* instance, PhysicalDevice* physical_device);
void device_free(Device* device, Instance* instance);

#endif