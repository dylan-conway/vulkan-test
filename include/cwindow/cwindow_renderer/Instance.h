#ifndef INSTANCE_H
#define INSTANCE_H

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include "dc_utils.h"
#include "gvec.h"

typedef struct Instance
{
    VkInstance handle;
    gvec layers;
    gvec extensions;

    #ifdef DEBUG
    VkDebugUtilsMessengerEXT debug_utils_messenger;
    #endif
} Instance;

Instance* instance_init(SDL_Window* window, const char* app_name, const char* engine_name);
void instance_free(Instance* instance);
void instance_setup_debug_utils(Instance* instance);

#endif