/*
    ISPPJ1 2024
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of a pong game and the declaration
    of the functions to init it, update it, and render it.
*/

#include <src/paddle.h>
#include <src/ball.h>
#include <src/sounds.h>

enum PongState
{
    START,
    SERVE,
    PLAY,
    MENU,
    DONE
};

struct Pong
{
    struct Paddle player1;
    struct Paddle player2;
    struct Ball ball;

    enum PongState state;

    int player1_score;
    int player2_score;
    int serving_player;
    int winning_player;

    float x1;
    float y1;
    float x2;
    float y2;

    bool CPU;
    bool cycle;

    struct Sounds* sounds;
};

void init_pong(struct Pong* pong, struct Sounds* sounds);

void handle_input_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state);

void update_pong(struct Pong* pong, double dt);

void render_pong(struct Pong pong, struct Fonts fonts);

void cpu_ia (struct Pong* pong);