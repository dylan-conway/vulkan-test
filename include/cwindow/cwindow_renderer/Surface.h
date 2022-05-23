#ifndef SURFACE_H
#define SURFACE_H

#include <vulkan/vulkan.h>
#include <SDL2/SDL_vulkan.h>
#include "Instance.h"
#include "PhysicalDevice.h"
#include "cwindow_renderer_base.h"

struct Surface
{
    VkSurfaceKHR handle;
    SDL_Window* window;
    VkSurfaceCapabilitiesKHR capabilities;
};

typedef struct I_Surface
{
    struct Surface* (*init)(struct Instance* instance, SDL_Window* window);
    void (*destroy)(struct Surface* surface, struct Instance* instance);
    void (*update_capabilities)(struct Surface* surface, struct PhysicalDevice* physical_device);
} I_Surface;

const I_Surface* Surface(void);

#endif