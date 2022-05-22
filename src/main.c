#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cwindow/cwindow.h"
#include "dc_utils.h"
#include "gvec.h"
#include "obj.h"

void init(cwindow* window, cwindow_renderer* renderer, cwindow_input* input)
{
    INFO_LOG("window %dx%d", window->get_width(), window->get_height());

    Player* p = calloc(1, sizeof(Player));
    {
        Player temp = player_impl()->init();
        *p = temp;
    }

    player_impl()->free(p);
    free(p);
}

void update(cwindow* window, cwindow_renderer* renderer, cwindow_input* input, f64 dt)
{

}

void render(cwindow* window, cwindow_renderer* renderer, cwindow_input* input)
{
    // renderer->clear();

    // renderer->set_draw_color(CW_GREEN);

    renderer->present();
}

int main(int argc, char** argv)
{
    cwindow_info cw_info = { "test", 640, 480, init, update, render };
    cwindow_start(cw_info);

    return 0;
}
