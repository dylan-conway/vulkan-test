#ifndef OBJ_H
#define OBJ_H

#include "dc_utils.h"
#include "cwindow/cwindow.h"
#include "cwindow/cwindow_renderer.h"

typedef struct Player
{
    i32 x, y;
    i32 width, height;
} Player;

typedef struct PlayerImpl
{
    Player (*init)(void);
    void (*free)(Player* player);
    void (*print)(Player* player);
    void (*update)(Player* player, cwindow* window, cwindow_input* input, f64 dt);
    void (*render)(Player* player, cwindow_renderer* renderer);
} PlayerImpl;

PlayerImpl* player_impl(void);

#endif