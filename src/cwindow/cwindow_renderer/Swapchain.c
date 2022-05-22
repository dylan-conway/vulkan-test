#include "cwindow/cwindow_renderer/Swapchain.h"

VkExtent2D get_swapchain_image_extent(VkSurfaceCapabilitiesKHR capabilities, SDL_Window* window);
VkSurfaceFormatKHR get_swapchain_image_format(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
VkPresentModeKHR get_swapchain_image_present_mode(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
u32 get_swapchain_image_count(VkSurfaceCapabilitiesKHR capabilities);

Swapchain* swapchain_init(Instance* instance, Device* device, PhysicalDevice* physical_device, Surface* surface)
{
    Swapchain* swapchain = calloc(1, sizeof(Swapchain));

    swapchain->image_extent = get_swapchain_image_extent(surface->capabilities, surface->window);
    swapchain->image_count = get_swapchain_image_count(surface->capabilities);
    swapchain->image_format = get_swapchain_image_format(physical_device->handle, surface->handle);

    VkSwapchainCreateInfoKHR swapchain_ci = { 0 };
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.surface = surface->handle;
    swapchain_ci.minImageCount = swapchain->image_count;
    swapchain_ci.imageFormat = swapchain->image_format.format;
    swapchain_ci.imageColorSpace = swapchain->image_format.colorSpace;
    swapchain_ci.imageExtent = swapchain->image_extent;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Present and graphics queues need to have same index for this to work
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;

    swapchain_ci.preTransform = surface->capabilities.currentTransform;
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_ci.presentMode = swapchain->present_mode;
    swapchain_ci.clipped = VK_TRUE;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK(vkCreateSwapchainKHR(device->handle, &swapchain_ci, NULL, &swapchain->handle));

    VK_CHECK(vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &swapchain->image_count, NULL));
    swapchain->images = malloc(sizeof(VkImage) * swapchain->image_count);
    swapchain->views = malloc(sizeof(VkImageView) * swapchain->image_count);

    VK_CHECK(vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &swapchain->image_count, swapchain->images));

    for (u32 i = 0; i < swapchain->image_count; i ++)
    {
        VkImageViewCreateInfo image_view_ci = { 0 };
        image_view_ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_ci.image = swapchain->images[i];
        image_view_ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_ci.format = swapchain->image_format.format;
        image_view_ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_ci.subresourceRange.baseMipLevel = 0;
        image_view_ci.subresourceRange.levelCount = 1;
        image_view_ci.subresourceRange.baseArrayLayer = 0;
        image_view_ci.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(device->handle, &image_view_ci, NULL, &swapchain->views[i]));
    }

    return swapchain;
}

void swapchain_free(Swapchain* swapchain, Device* device)
{
    for (u32 i = 0; i < swapchain->image_count; i ++)
    {
        vkDestroyImageView(device->handle, swapchain->views[i], NULL);
    }
    free(swapchain->images);
    free(swapchain->views);
    vkDestroySwapchainKHR(device->handle, swapchain->handle, NULL);
    free(swapchain);
}

VkExtent2D get_swapchain_image_extent(VkSurfaceCapabilitiesKHR capabilities, SDL_Window* window)
{
    VkExtent2D extent = { 0 };
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        extent = capabilities.currentExtent;
    }
    else
    {
        i32 width, height;
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        u32 min_width = capabilities.minImageExtent.width;
        u32 min_height = capabilities.minImageExtent.height;
        u32 max_width = capabilities.maxImageExtent.width;
        u32 max_height = capabilities.maxImageExtent.height;

        extent.width = (u32)width < min_width ? min_width : (u32)width > max_width ? max_width : (u32)width;
        extent.height = (u32)height < min_height ? min_height : (u32)height > max_height ? max_height : (u32)height;
    }
}

VkSurfaceFormatKHR get_swapchain_image_format(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    u32 formats_count = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formats_count, NULL));
    VkSurfaceFormatKHR* formats = malloc(sizeof(VkSurfaceFormatKHR) * formats_count);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formats_count, formats));

    VkSurfaceFormatKHR format = { 0 };

    // TODO ("check when desired format is not found")
    for (u32 i = 0; i < formats_count; i ++)
    {
        if (VK_FORMAT_B8G8R8A8_SRGB == formats[i].format && VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == formats[i].colorSpace)
        {
            format = formats[i];
            break;
        }
    }

    free(formats);
    return format;
}

VkPresentModeKHR get_swapchain_image_present_mode(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    u32 present_modes_count = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_modes_count, NULL));
    VkPresentModeKHR* present_modes = malloc(sizeof(VkPresentModeKHR) * present_modes_count);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_modes_count, present_modes));

    VkPresentModeKHR present_mode = present_modes[0];

    // TODO ("check when desired present mode is not found")
    for (u32 i = 0; i < present_modes_count; i ++)
    {
        if (VK_PRESENT_MODE_MAILBOX_KHR == present_modes[i])
        {
            present_mode = present_modes[i];
            break;
        }
    }

    free(present_modes);
    return present_mode;
}

u32 get_swapchain_image_count(VkSurfaceCapabilitiesKHR capabilities)
{
    u32 count = capabilities.minImageCount + 1;
    if (0 != capabilities.minImageCount && count > capabilities.maxImageCount)
    {
        count = capabilities.maxImageCount;
    }

    return count;
}

