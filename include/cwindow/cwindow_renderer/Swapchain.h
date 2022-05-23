#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "Device.h"
#include "Semaphore.h"

struct Swapchain
{
    VkSwapchainKHR handle;
    VkImage* images;
    VkImageView* views;
    VkSurfaceFormatKHR image_format;
    VkPresentModeKHR present_mode;
    VkExtent2D image_extent;
    u32 image_count;
    bool recreation_required;
    bool window_resized;
};

typedef struct I_Swapchain
{
    struct Swapchain* (*init)(struct Device* device, struct PhysicalDevice* physical_device, struct Surface* surface);
    void (*destroy)(struct Swapchain* swapchain, struct Device* device);
    u32 (*acquire_next_image)(struct Swapchain* swapchain, struct Device* device, struct Semaphore* image_available_semaphore);
} I_Swapchain;

const I_Swapchain* Swapchain(void);

#endif