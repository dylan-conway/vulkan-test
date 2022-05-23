#include "cwindow/cwindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#define METHOD(t, m) t##____##m

static SDL_Window* sdl_window = NULL;
static i32 width = 0;
static i32 height = 0;
static bool running = true;

void METHOD(cwindow, set_size)(i32 new_width, i32 new_height);
i32 METHOD(cwindow, get_width)(void);
void METHOD(cwindow, set_width)(i32 new_width);
i32 METHOD(cwindow, get_height)(void);
void METHOD(cwindow, set_height)(i32 new_height);
void METHOD(cwindow, shutdown)(void);

cwindow cwindow_init(void);

void cwindow_start(cwindow_info info)
{
    if (NULL == info.update)
    {
        ERR_LOG("missing update function");
        return;
    }
    if (NULL == info.render)
    {
        ERR_LOG("missing render function");
        return;
    }

    width = info.width;
    height = info.height;

    cwindow window = cwindow_init();

    if (0 != SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_ERR_LOG("failed to initialize SDL2 with everything: \"%s\"", SDL_GetError());
    }
    
    sdl_window = SDL_CreateWindow(info.title, 1700, 50, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (NULL == sdl_window)
    {
        SDL_ERR_LOG("failed to create SDL2 sdl_window: \"%s\"", SDL_GetError());
    }

    cwindow_renderer renderer = cwindow_renderer_init(sdl_window);

    if (NULL == info.init)
    {
        WARN_LOG("missing init definition");
    }
    else
    {
        info.init(&window, &renderer, NULL);
    }

    u64 curr_time = get_counter();
    u64 prev_time = get_counter();

    while (running)
    {
        prev_time = curr_time;
        curr_time = get_counter();

        SDL_Event event = { 0 };
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_MINIMIZED:
                            cwindow_renderer_handle_minimized();
                            break;
                        case SDL_WINDOWEVENT_RESTORED:
                            cwindow_renderer_handle_restored();
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                            SDL_GetWindowSize(sdl_window, &width, &height);
                            cwindow_renderer_handle_resized();
                            break;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    break;
            }
        }

        info.update(&window, &renderer, NULL, get_delta2(prev_time, curr_time, SECONDS));

        info.render(&window, &renderer, NULL);
    }

    cwindow_renderer_free();
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

cwindow cwindow_init(void)
{
    cwindow window = {
        .set_size = METHOD(cwindow, set_size),
        .get_width = METHOD(cwindow, get_width),
        .set_width = METHOD(cwindow, set_width),
        .get_height = METHOD(cwindow, get_height),
        .set_height = METHOD(cwindow, set_height),
        .shutdown = METHOD(cwindow, shutdown)
    };

    return window;
}

void METHOD(cwindow, set_size)(i32 new_width, i32 new_height)
{
    width = new_width;
    height = new_height;
    SDL_SetWindowSize(sdl_window, width, height);
}

i32 METHOD(cwindow, get_width)(void)
{
    return width;
}

void METHOD(cwindow, set_width)(i32 new_width)
{
    width = new_width;
    SDL_SetWindowSize(sdl_window, width, height);
}

i32 METHOD(cwindow, get_height)(void)
{
    return height;
}

void METHOD(cwindow, set_height)(i32 new_height)
{
    height = new_height;
    SDL_SetWindowSize(sdl_window, width, height);
}

void METHOD(cwindow, shutdown)(void)
{
    running = false;
}