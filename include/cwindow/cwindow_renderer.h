#ifndef CWINDOW_RENDERER_H
#define CWINDOW_RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h.>

#include "dc_utils.h"

typedef struct cwindow_renderer
{
    void (*clear)(void);
    void (*present)(void);
} cwindow_renderer;

cwindow_renderer cwindow_renderer_init(void* sdl_window);
void cwindow_renderer_free(void);
void cwindow_renderer_handle_resize(void);

#endif