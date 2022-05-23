#include "cwindow/cwindow_renderer/Surface.h"
#include "dc_utils.h"

// TODO ("potentially move entire surface into Instance");
static struct Surface* init(struct Instance* instance, SDL_Window* window)
{
    struct Surface* surface = calloc(1, sizeof(struct Surface));

    surface->window = window;

    if (SDL_FALSE == SDL_Vulkan_CreateSurface(window, instance->handle, &surface->handle))
    {
        SDL_ERR_LOG("failed to create Vulkan surface for SDL2 window, \"%s\"", SDL_GetError());
    }

    return surface;
}

static void destroy(struct Surface* surface, struct Instance* instance)
{
    vkDestroySurfaceKHR(instance->handle, surface->handle, NULL);
    free(surface);
}

static void update_capabilities(struct Surface* surface, struct PhysicalDevice* physical_device)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->handle, surface->handle, &surface->capabilities);
}

static I_Surface I_SURFACE = {
    .init = init,
    .destroy = destroy,
    .update_capabilities = update_capabilities,
};

const I_Surface* Surface(void)
{
    return &I_SURFACE;
}