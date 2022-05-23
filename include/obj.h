#ifndef OBJ_H
#define OBJ_H

#include "dc_utils.h"
#include "cwindow/cwindow.h"
#include "cwindow/cwindow_renderer.h"

struct Player
{
    i32 x, y;
    i32 width, height;
};

typedef struct IPlayer
{
    struct Player (*init)(void);
    void (*free)(struct Player* player);
    void (*print)(struct Player* player);
    void (*update)(struct Player* player, cwindow* window, cwindow_input* input, f64 dt);
    void (*render)(struct Player* player, cwindow_renderer* renderer);
} IPlayer;

IPlayer* Player(void);

#endif