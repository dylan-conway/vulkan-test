#include "obj.h"

#define METHOD(t, m) t##____##m

Player METHOD(Player, init)(void);
void METHOD(Player, free)(Player* player);
void METHOD(Player, print)(Player* player);
void METHOD(Player, update)(Player* player, cwindow* window, cwindow_input* input, f64 dt);
void METHOD(Player, render)(Player* player, cwindow_renderer* renderer);

static PlayerImpl PLAYER_IMPL = {
    .init = METHOD(Player, init),
    .free = METHOD(Player, free),
    .print = METHOD(Player, print),
    .update = METHOD(Player, update),
    .render = METHOD(Player, render),
};

PlayerImpl* player_impl(void)
{
    return &PLAYER_IMPL;
}

Player METHOD(Player, init)(void)
{
    Player p = { 0 };
    return p;
}

void METHOD(Player, free)(Player* player)
{

}

void METHOD(Player, print)(Player* player)
{

}

void METHOD(Player, update)(Player* player, cwindow* window, cwindow_input* input, f64 dt)
{

}

void METHOD(Player, render)(Player* player, cwindow_renderer* renderer)
{

}
