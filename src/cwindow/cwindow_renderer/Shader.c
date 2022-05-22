#include "cwindow/cwindow_renderer/Shader.h"
#include "dc_utils.h"

Shader* shader_init(Device* device, shaderc_shader_kind shader_type, const str shader_path, const str entry_point)
{
    Shader* shader = calloc(1, sizeof(Shader));
    shader->entry_point = entry_point;

    shaderc_compiler_t compiler = shaderc_compiler_initialize();
    if (NULL == compiler)
    {
        ERR_LOG("failed to initialize shaderc");
    }

    shaderc_compile_options_t options = shaderc_compile_options_initialize();
    if (NULL == options)
    {
        ERR_LOG("failed to initialize shaderc options");
    }

    // shaderc_compile_options_set_warnings_as_errors(options);

    size_t source_length = 0;
    str source = read_file(shader_path, &source_length);

    const shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, source, source_length, shader_type, shader_path, (const str)shader->entry_point, options);
    shaderc_compilation_status status = shaderc_result_get_compilation_status(result);
    if (shaderc_compilation_status_success == status)
    {
        SUCCESS_LOG("shader compiled: \"%s\"\n", shader_path);
    }
    else
    {
        ERR_LOG("Shader compiled: \"%s\". Errors: %zu. Warnings: %zu", shader_path, shaderc_result_get_num_errors(result), shaderc_result_get_num_warnings(result));
        ERR_LOG("%s", shaderc_result_get_error_message(result));
    }

    VkShaderModuleCreateInfo shader_ci = { 0 };
    shader_ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_ci.codeSize = shaderc_result_get_length(result);
    shader_ci.pCode = (const u32*)shaderc_result_get_bytes(result);

    VK_CHECK(vkCreateShaderModule(device->handle, &shader_ci, NULL, &shader->handle));

    shaderc_result_release(result);
    shaderc_compile_options_release(options);
    shaderc_compiler_release(compiler);

    return shader;
}

void shader_free(Shader* shader, Device* device)
{
    vkDestroyShaderModule(device->handle, shader->handle, NULL);
    free(shader);
}
