#include "cwindow/cwindow_renderer/Device.h"

static void get_device_extensions(dvec_str extensions);

static struct Device* init(struct PhysicalDevice* physical_device)
{
    struct Device* device = calloc(1, sizeof(struct Device));

    device->extensions = dvec_init(str);
    get_device_extensions(device->extensions);

    if (physical_device->graphics_queue_index != physical_device->present_queue_index)
    {
        ERR_LOG("graphics and present queue indicies not equal, graphics[%u] present[%u]", physical_device->graphics_queue_index, physical_device->present_queue_index);
    }

    f32 queue_priority = 1.0f;
    VkDeviceQueueCreateInfo graphics_queue_ci = { 0 }; // and present
    graphics_queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphics_queue_ci.queueFamilyIndex = physical_device->graphics_queue_index;
    graphics_queue_ci.queueCount = 1;
    graphics_queue_ci.pQueuePriorities = &queue_priority;

    VkDeviceQueueCreateInfo queue_infos[] = {
        graphics_queue_ci,
    };

    VkPhysicalDeviceFeatures device_features = { 0 };
    device_features.samplerAnisotropy = VK_TRUE;
    if (VK_FALSE == physical_device->features.samplerAnisotropy)
    {
        ERR_LOG("physical device does not support samplerAnisotropy");
    }

    VkDeviceCreateInfo device_ci = { 0 };
    device_ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_ci.queueCreateInfoCount = 1;
    device_ci.pQueueCreateInfos = queue_infos;
    device_ci.pEnabledFeatures = &device_features;
    device_ci.enabledExtensionCount = dvec_size(device->extensions);
    device_ci.ppEnabledExtensionNames = (const char* const*)dvec_data(device->extensions);

    VK_CHECK(vkCreateDevice(physical_device->handle, &device_ci, NULL, &device->handle));
    vkGetDeviceQueue(device->handle, physical_device->graphics_queue_index, 0, &device->graphics_queue);
    vkGetDeviceQueue(device->handle, physical_device->present_queue_index, 0, &device->present_queue);

    INFO_LOG("Enabled Vulkan device extensions <dvec_str>:");
    for (u64 i = 0; i < dvec_size(device->extensions); i ++)
    {
        INFO_LOG("    %zu: %s", i, dvec_data(device->extensions)[i]);
    }

    return device;
}

static void destroy(struct Device* device)
{
    dvec_free(device->extensions);
    vkDestroyDevice(device->handle, NULL);
    free(device);
}

static void get_device_extensions(dvec_str extensions)
{
    dvec_append(extensions, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    // gvec_append(extensions, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
}

static I_Device I_DEVICE = {
    .init = init,
    .destroy = destroy,
};

const I_Device* Device(void)
{
    return &I_DEVICE;
}