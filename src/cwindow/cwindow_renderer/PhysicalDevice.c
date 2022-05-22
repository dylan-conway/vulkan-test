#include "cwindow/cwindow_renderer/PhysicalDevice.h"
#include "cwindow/cwindow_renderer/Instance.h"
#include "cwindow/cwindow_renderer/Surface.h"

PhysicalDevice* physical_device_init(Instance* instance, Surface* surface)
{
    PhysicalDevice* physical_device = calloc(1, sizeof(PhysicalDevice));

    {
        u32 count = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(instance->handle, &count, NULL));
        VkPhysicalDevice* physical_devices = malloc(sizeof(VkPhysicalDevice) * count);
        VK_CHECK(vkEnumeratePhysicalDevices(instance->handle, &count, physical_devices));

        // TODO
        physical_device->handle = physical_devices[0];
        free(physical_devices);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->handle, surface->handle, &surface->capabilities);
    vkGetPhysicalDeviceProperties(physical_device->handle, &physical_device->properties);
    vkGetPhysicalDeviceMemoryProperties(physical_device->handle, &physical_device->memory_properties);
    vkGetPhysicalDeviceFeatures(physical_device->handle, &physical_device->features);

    {
        u32 count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device->handle, &count, NULL);
        VkQueueFamilyProperties* queue_families = malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device->handle, &count, queue_families);

        for (u32 i = 0; i < count; i ++)
        {
            VkBool32 is_present_supported = VK_FALSE;
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device->handle, i, surface->handle, &is_present_supported));
            if ((VK_QUEUE_GRAPHICS_BIT & queue_families[i].queueFlags) && is_present_supported)
            {
                physical_device->graphics_queue_index = i;
                physical_device->present_queue_index = i;
                break;
            }
        }

        free(queue_families);
    }

    return physical_device;
}

void physical_device_free(PhysicalDevice* physical_device)
{
    free(physical_device);
}