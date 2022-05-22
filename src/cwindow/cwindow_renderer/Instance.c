#include "cwindow/cwindow_renderer/Instance.h"
#include <SDL2/SDL_vulkan.h>
#include "cwindow/cwindow_renderer/Surface.h"
#include "dc_utils.h"

#ifdef DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

PFN_vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT _vkDestroyDebugUtilsMessengerEXT;

VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_ci = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .pNext = NULL,
    .flags = 0,
    .messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |*/ VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = debug_utils_callback,
    .pUserData = NULL,
};
#endif

/////////////////// private methods begin

void get_instance_layers(Instance* instance);
void get_instance_extensions(Instance* instance, SDL_Window* window);

/////////////////// private methods end

Instance* instance_init(SDL_Window* window, const char* app_name, const char* engine_name)
{
    Instance* instance = calloc(1, sizeof(Instance));

    instance->layers = gvec_init(gvec_str);
    instance->extensions = gvec_init(gvec_str);
    get_instance_layers(instance);
    get_instance_extensions(instance, window);

    VkApplicationInfo app_info = { 0 };
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = engine_name;
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    char** layer_names = NULL;
    gvec_data(instance->layers, &layer_names);
    char** extension_names = NULL;
    gvec_data(instance->extensions, &extension_names);

    VkInstanceCreateInfo instance_ci = { 0 };
    instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_ci.pApplicationInfo = &app_info;
    instance_ci.enabledLayerCount = gvec_size(instance->layers);
    instance_ci.ppEnabledLayerNames = (const char* const*)layer_names;
    instance_ci.enabledExtensionCount = gvec_size(instance->extensions);
    instance_ci.ppEnabledExtensionNames = (const char* const*)extension_names;

    #ifdef DEBUG
    instance_ci.pNext = &debug_utils_messenger_ci;
    #endif

    VK_CHECK(vkCreateInstance(&instance_ci, NULL, &instance->handle));

    gvec_print(instance->layers, "Enabled Vulkan instance layers");
    gvec_print(instance->extensions, "Enabled Vulkan instance extensions");

    return instance;
}

void instance_free(Instance* instance)
{
    gvec_free(instance->layers);
    gvec_free(instance->extensions);
    #ifdef DEBUG
    _vkDestroyDebugUtilsMessengerEXT(instance->handle, instance->debug_utils_messenger, NULL);
    #endif
    vkDestroyInstance(instance->handle, NULL);
    free(instance);
}

void instance_setup_debug_utils(Instance* instance)
{
    #ifdef DEBUG
    _vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance->handle, "vkCreateDebugUtilsMessengerEXT");
    _vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance->handle, "vkDestroyDebugUtilsMessengerEXT");
    VK_CHECK(_vkCreateDebugUtilsMessengerEXT(instance->handle, &debug_utils_messenger_ci, NULL, &instance->debug_utils_messenger));
    #endif
}

/////////////////// private methods begin

void get_instance_layers(Instance* instance)
{
    #ifdef DEBUG
    gvec_append(instance->layers, "VK_LAYER_KHRONOS_validation");
    #endif
}

void get_instance_extensions(Instance* instance, SDL_Window* window)
{
    u32 count = 0;
    if (SDL_FALSE == SDL_Vulkan_GetInstanceExtensions(window, &count, NULL))
    {
        SDL_ERR_LOG("failed to get number of instance extensions, \"%s\"", SDL_GetError());
    }
    const char** required_instance_extensions = malloc(sizeof(char*) * count);
    if (SDL_FALSE == SDL_Vulkan_GetInstanceExtensions(window, &count, required_instance_extensions))
    {
        SDL_ERR_LOG("failed to get instance extensions count[%u], \"%s\"", count, SDL_GetError());
    }

    for (u32 i = 0; i < count; i ++)
    {
        gvec_append(instance->extensions, (char*)required_instance_extensions[i]);
    }

    #ifdef DEBUG
    gvec_append(instance->extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif

    free(required_instance_extensions);
}

/////////////////// private methods end

#ifdef DEBUG
char* get_debug_message_color(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
char get_debug_message_severity_tag(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
char get_debug_message_type_tag(VkDebugUtilsMessageTypeFlagBitsEXT type);

VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    char timestamp[TIMESTAMP_STR_MAX_LEN] = { '\0' };
    get_timestamp(timestamp);

    char* message_color = get_debug_message_color(messageSeverity);
    char severity_tag = get_debug_message_severity_tag(messageSeverity);
    char type_tag = get_debug_message_type_tag(messageType);

    // TODO ("pretty print validation output, maybe just for validation errors because of the format")
    fprintf(stdout, "%s  %sVK Validation [%c:%c]" TERM_RESET_COLOR " >>>  %s\n", timestamp, message_color, severity_tag, type_tag, pCallbackData->pMessage);

    return VK_FALSE;
}

char* get_debug_message_color(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
{
    switch(severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return TERM_WHITE;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return TERM_RED;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return TERM_YELLOW;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return TERM_RED;
        default: return TERM_WHITE;
    }

    return TERM_WHITE;
}

char get_debug_message_severity_tag(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
{
    switch(severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return 'V';
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return 'I';
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return 'W';
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return 'E';
        default: return '?';
    }

    return '?';
}

char get_debug_message_type_tag(VkDebugUtilsMessageTypeFlagBitsEXT type)
{
    switch(type)
    {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return 'G';
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return 'V';
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return 'P';
        default: return '?';
    }

    return '?';
}

#endif