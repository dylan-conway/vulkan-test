#ifndef INSTANCE_H
#define INSTANCE_H

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include "dc_utils.h"
#include "dvec.h"

struct Instance
{
    VkInstance handle;
    dvec_str layers;
    dvec_str extensions;

    #ifdef DEBUG
    VkDebugUtilsMessengerEXT debug_utils_messenger;
    #endif
};

typedef struct I_Instance
{
    struct Instance* (*init)(SDL_Window* window, const char* app_name, const char* engine_name);
    void (*destroy)(struct Instance* instance);
    void (*setup_debug_utils)(struct Instance* instance);
} I_Instance;

const I_Instance* Instance(void);

#endif