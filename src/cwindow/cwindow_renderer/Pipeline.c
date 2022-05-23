#include "cwindow/cwindow_renderer/Pipeline.h"
#include "cwindow/cwindow_renderer/Shader.h"

static struct Pipeline* init(struct Device* device, struct Swapchain* swapchain)
{
    struct Pipeline* pipeline = calloc(1, sizeof(struct Pipeline));

    VkAttachmentDescription color_attachment = { 0 };
    color_attachment.format = swapchain->image_format.format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = { 0 };
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_desc = { 0 };
    subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_desc.colorAttachmentCount = 1;
    subpass_desc.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency subpass_dependency = { 0 };
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_ci = { 0 };
    render_pass_ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_ci.attachmentCount = 1;
    render_pass_ci.pAttachments = &color_attachment;
    render_pass_ci.subpassCount = 1;
    render_pass_ci.pSubpasses = &subpass_desc;
    render_pass_ci.dependencyCount = 1;
    render_pass_ci.pDependencies = &subpass_dependency;

    VK_CHECK(vkCreateRenderPass(device->handle, &render_pass_ci, NULL, &pipeline->render_pass));

    // Descriptor set layouts...
    VkPipelineLayoutCreateInfo pipeline_layout_ci = { 0 };
    pipeline_layout_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_ci.setLayoutCount = 0;
    pipeline_layout_ci.pSetLayouts = NULL;

    VK_CHECK(vkCreatePipelineLayout(device->handle, &pipeline_layout_ci, NULL, &pipeline->layout));

    Shader* vert_shader = shader_init(device, shaderc_vertex_shader, "../shaders/vert_main.glsl", "main");
    Shader* frag_shader = shader_init(device, shaderc_fragment_shader, "../shaders/frag_main.glsl", "main");

    VkPipelineShaderStageCreateInfo vert_shader_stage_ci = { 0 };
    vert_shader_stage_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_ci.module = vert_shader->handle;
    vert_shader_stage_ci.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_ci.pName = vert_shader->entry_point;

    VkPipelineShaderStageCreateInfo frag_shader_stage_ci = { 0 };
    frag_shader_stage_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_ci.module = frag_shader->handle;
    frag_shader_stage_ci.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_ci.pName = frag_shader->entry_point;

    VkPipelineShaderStageCreateInfo shader_stages[] = {
        vert_shader_stage_ci,
        frag_shader_stage_ci
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_info = { 0 };
    vertex_input_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_ci = { 0 };
    input_assembly_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state_ci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_state_ci.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = { 0 };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain->image_extent.width;
    viewport.height = (float)swapchain->image_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = { 0 };
    scissor.extent = swapchain->image_extent;

    VkPipelineViewportStateCreateInfo viewport_state_ci = { 0 };
    viewport_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_ci.viewportCount = 1;
    viewport_state_ci.pViewports = &viewport;
    viewport_state_ci.scissorCount = 1;
    viewport_state_ci.pScissors = &scissor;
    
    VkPipelineRasterizationStateCreateInfo rasterization_state_ci = { 0 };
    rasterization_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_ci.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state_ci.lineWidth = 1.0f;
    rasterization_state_ci.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterization_state_ci.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisample_state_ci = { 0 };
    multisample_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_ci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_state_ci.minSampleShading = 1.0f;

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = { 0 };
    color_blend_attachment_state.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment_state.blendEnable = VK_FALSE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend_state_ci = { 0 };
    color_blend_state_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_ci.logicOp = VK_LOGIC_OP_COPY;
    color_blend_state_ci.attachmentCount = 1;
    color_blend_state_ci.pAttachments = &color_blend_attachment_state;
    color_blend_state_ci.blendConstants[0] = 0.0f;
    color_blend_state_ci.blendConstants[1] = 0.0f;
    color_blend_state_ci.blendConstants[2] = 0.0f;
    color_blend_state_ci.blendConstants[3] = 0.0f;

    VkGraphicsPipelineCreateInfo graphics_pipeline_ci = { 0 };
    graphics_pipeline_ci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_ci.stageCount = 2;
    graphics_pipeline_ci.pStages = shader_stages;
    graphics_pipeline_ci.pVertexInputState = &vertex_input_state_info;
    graphics_pipeline_ci.pInputAssemblyState = &input_assembly_state_ci;
    graphics_pipeline_ci.pViewportState = &viewport_state_ci;
    graphics_pipeline_ci.pRasterizationState = &rasterization_state_ci;
    graphics_pipeline_ci.pMultisampleState = &multisample_state_ci;
    graphics_pipeline_ci.pDepthStencilState = NULL;
    graphics_pipeline_ci.pColorBlendState = &color_blend_state_ci;
    graphics_pipeline_ci.pDynamicState = NULL;
    graphics_pipeline_ci.layout = pipeline->layout;
    graphics_pipeline_ci.renderPass = pipeline->render_pass;
    graphics_pipeline_ci.subpass = 0;
    graphics_pipeline_ci.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_ci.basePipelineIndex = -1;

    VK_CHECK(vkCreateGraphicsPipelines(device->handle, VK_NULL_HANDLE, 1, &graphics_pipeline_ci, NULL, &pipeline->handle));

    shader_free(vert_shader, device);
    shader_free(frag_shader, device);

    pipeline->framebuffers_count = swapchain->image_count;
    pipeline->framebuffers = malloc(sizeof(VkFramebuffer) * pipeline->framebuffers_count);
    for (u32 i = 0; i < pipeline->framebuffers_count; i ++)
    {
        VkFramebufferCreateInfo framebuffer_ci = { 0 };
        framebuffer_ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_ci.renderPass = pipeline->render_pass;
        framebuffer_ci.attachmentCount = 1;
        framebuffer_ci.pAttachments = &swapchain->views[i];
        framebuffer_ci.width = swapchain->image_extent.width;
        framebuffer_ci.height = swapchain->image_extent.height;
        framebuffer_ci.layers = 1;

        VK_CHECK(vkCreateFramebuffer(device->handle, &framebuffer_ci, NULL, &pipeline->framebuffers[i]));
    }

    return pipeline;
}

static void destroy(struct Pipeline* pipeline, struct Device* device)
{
    for (u32 i = 0; i < pipeline->framebuffers_count; i ++)
    {
        vkDestroyFramebuffer(device->handle, pipeline->framebuffers[i], NULL);
    }
    free(pipeline->framebuffers);
    vkDestroyPipelineLayout(device->handle, pipeline->layout, NULL);
    vkDestroyRenderPass(device->handle, pipeline->render_pass, NULL);
    vkDestroyPipeline(device->handle, pipeline->handle, NULL);
    free(pipeline);
}

static I_Pipeline I_PIPELINE = {
    .init = init,
    .destroy = destroy,
};

const I_Pipeline* Pipeline(void)
{
    return &I_PIPELINE;
}
