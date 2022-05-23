#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cwindow/cwindow.h"
#include "dc_utils.h"
#include "gvec.h"
#include "vec.h"
#include "obj.h"

void init(cwindow* window, cwindow_renderer* renderer, cwindow_input* input)
{
    INFO_LOG("window %dx%d", window->get_width(), window->get_height());

    struct Player p = Player()->init();
}

void update(cwindow* window, cwindow_renderer* renderer, cwindow_input* input, f64 dt)
{
    // INFO_LOG("dt: %f", dt);
}

void render(cwindow* window, cwindow_renderer* renderer, cwindow_input* input)
{
    renderer->clear(CW_MEDIUMPURPLE);

    // renderer->set_draw_color(CW_GREEN);

    renderer->present();
}

int main(int argc, char** argv)
{

    vec_i32 int_vector = vec_init(i32, 1);
    vec_resize(int_vector, 2);
    vec_free(int_vector);

    cwindow_info cw_info = { "test", 640, 480, init, update, render };
    cwindow_start(cw_info);

    return 0;
}
