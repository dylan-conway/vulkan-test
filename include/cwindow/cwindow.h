#ifndef CWINDOW_H
#define CWINDOW_H

#include "dc_utils.h"
#include "cwindow/cwindow_types.h"
#include "cwindow/cwindow_renderer.h"

typedef struct cwindow_input
{
    
} cwindow_input;

typedef struct cwindow
{
    //! @brief Set width and height of window
    void (*set_size)(i32 new_width, i32 new_height);

    //! @brief Get the current width of the window
    i32 (*get_width)(void);

    //! @brief Set the width of the window
    void (*set_width)(i32 new_width);

    //! @brief Get the current height of the window
    i32 (*get_height)(void);

    //! @brief Set the height of the window
    void (*set_height)(i32 new_height);

    //! @brief Close window
    void (*shutdown)(void);
} cwindow;

typedef struct cwindow_info
{
    const char* title;
    i32 width, height;
    void (*init)(cwindow* window, cwindow_renderer* renderer, cwindow_input* input);
    void (*update)(cwindow* window, cwindow_renderer* renderer, cwindow_input* input, double dt);
    void (*render)(cwindow* window, cwindow_renderer* renderer, cwindow_input* input);
} cwindow_info;

void cwindow_start(cwindow_info info);

#endif