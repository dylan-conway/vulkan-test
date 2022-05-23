#include "cwindow/cwindow_renderer/Semaphore.h"

static struct Semaphore* init(struct Device* device)
{
    struct Semaphore* semaphore = calloc(1, sizeof(struct Semaphore));

    VkSemaphoreCreateInfo semaphore_ci = { 0 };
    semaphore_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CHECK(vkCreateSemaphore(device->handle, &semaphore_ci, NULL, &semaphore->handle));

    return semaphore;
}

static void destroy(struct Semaphore* semaphore, struct Device* device)
{
    vkDestroySemaphore(device->handle, semaphore->handle, NULL);
    free(semaphore);
}

static I_Semaphore I_SEMAPHORE = {
    .init = init,
    .destroy = destroy,
};

const I_Semaphore* Semaphore(void)
{
    return &I_SEMAPHORE;
}