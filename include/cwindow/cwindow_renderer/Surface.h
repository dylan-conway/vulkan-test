#ifndef SURFACE_H
#define SURFACE_H

#include <vulkan/vulkan.h>
#include <SDL2/SDL_vulkan.h>
#include "Instance.h"

typedef struct Surface
{
    VkSurfaceKHR handle;
    SDL_Window* window;
    VkSurfaceCapabilitiesKHR capabilities;
} Surface;

Surface* surface_init(Instance* instance, SDL_Window* window);
void surface_free(Surface* surface, Instance* instance);

#endif