#include "cwindow/cwindow_renderer/Surface.h"
#include "dc_utils.h"


// TODO ("potentially move entire surface into Instance");
Surface* surface_init(Instance* instance, SDL_Window* window)
{
    Surface* surface = calloc(1, sizeof(Surface));

    surface->window = window;

    if (SDL_FALSE == SDL_Vulkan_CreateSurface(window, instance->handle, &surface->handle))
    {
        SDL_ERR_LOG("failed to create Vulkan surface for SDL2 window, \"%s\"", SDL_GetError());
    }

    return surface;
}

void surface_free(Surface* surface, Instance* instance)
{
    vkDestroySurfaceKHR(instance->handle, surface->handle, NULL);
    free(surface);
}