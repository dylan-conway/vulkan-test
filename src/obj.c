#include "obj.h"

static struct Player Player___init(void);
static void Player___free(struct Player* player);
static void Player___print(struct Player* player);
static void Player___update(struct Player* player, cwindow* window, cwindow_input* input, f64 dt);
static void Player___render(struct Player* player, cwindow_renderer* renderer);

static IPlayer I_PLAYER = {
    .init = Player___init,
    .free = Player___free,
    .print = Player___print,
    .update = Player___update,
    .render = Player___render,
};

IPlayer* Player(void)
{
    return &I_PLAYER;
}

struct Player Player___init(void)
{
    struct Player p = { 0 };
    return p;
}

void Player___free(struct Player* player)
{

}

void Player___print(struct Player* player)
{

}

void Player___update(struct Player* player, cwindow* window, cwindow_input* input, f64 dt)
{

}

void Player___render(struct Player* player, cwindow_renderer* renderer)
{

}
