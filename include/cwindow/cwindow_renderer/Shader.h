#ifndef SHADER_H
#define SHADER_H

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.h>

#include "Device.h"
#include "dc_utils.h"

typedef struct Shader
{
    VkShaderModule handle;
    str entry_point;
} Shader;

Shader* shader_init(struct Device* device, shaderc_shader_kind shader_type, const str shader_path, const str entry_point);
void shader_free(Shader* shader, struct Device* device);

#endif