#include "cwindow/cwindow_renderer/Surface.h"
#include "dc_utils.h"

static struct Surface* init(struct Instance* instance, SDL_Window* window);
static void destroy(struct Surface* surface, struct Instance* instance);

static I_Surface I_SURFACE = {
    .init = init,
    .destroy = destroy,
};

const I_Surface* Surface(void)
{
    return &I_SURFACE;
}

// TODO ("potentially move entire surface into Instance");
struct Surface* init(struct Instance* instance, SDL_Window* window)
{
    struct Surface* surface = calloc(1, sizeof(struct Surface));

    surface->window = window;

    if (SDL_FALSE == SDL_Vulkan_CreateSurface(window, instance->handle, &surface->handle))
    {
        SDL_ERR_LOG("failed to create Vulkan surface for SDL2 window, \"%s\"", SDL_GetError());
    }

    return surface;
}

void destroy(struct Surface* surface, struct Instance* instance)
{
    vkDestroySurfaceKHR(instance->handle, surface->handle, NULL);
    free(surface);
}