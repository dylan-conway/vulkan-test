#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "Device.h"

typedef struct Swapchain
{
    VkSwapchainKHR handle;
    VkImage* images;
    VkImageView* views;
    VkSurfaceFormatKHR image_format;
    VkPresentModeKHR present_mode;
    VkExtent2D image_extent;
    u32 image_count;
} Swapchain;

Swapchain* swapchain_init(Instance* instance, Device* device, PhysicalDevice* physical_device, Surface* surface);
void swapchain_free(Swapchain* swapchain, Device* device);

#endif