/*
    ISPPJ1 2024
    Study Case: Pong

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the functions to init a pong game,
    update it, and render it.
*/

#include <stdio.h>

#include <allegro5/allegro_primitives.h>

#include <settings.h>
#include <src/fonts.h>
#include <src/pong.h>

void init_pong(struct Pong* pong, struct Sounds* sounds)
{
    init_paddle(&pong->player1, PADDLE_X_OFFSET, PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_paddle(&pong->player2, TABLE_WIDTH - PADDLE_WIDTH - PADDLE_X_OFFSET, TABLE_HEIGHT - PADDLE_HEIGHT - PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
    pong->state = MENU;
    pong->player1_score = 0;
    pong->player2_score = 0;
    pong->serving_player = 0;
    pong->winning_player = 0;
    pong->sounds = sounds;
    srand(time(NULL));
}

void handle_input_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (pong->state == MENU)
    {
        if (al_key_down(state, ALLEGRO_KEY_1))
        {
            pong->CPU = true;
            pong->state = START;
        }
        if (al_key_down(state, ALLEGRO_KEY_2))
        {
            pong->state = START;
            pong->CPU = false;
        }
    }

    if (pong->state == START)
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
        {
            pong->state = SERVE;
            pong->serving_player = rand() % 2 + 1;
        }
    }
    else if (pong->state == SERVE)
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
        {
             pong->state = PLAY;

            pong->ball.vx = rand() % 60 + 140;

            if (pong->serving_player == 2)
            {
                pong->ball.vx *= -1;
            }

            pong->ball.vy = rand() % 100 - 50;
        }
    }
    else if (pong->state == PLAY)
    {
        if (al_key_down(state, ALLEGRO_KEY_S))
        {
            pong->player1.vy = PADDLE_SPEED;
        }
        else if (al_key_down(state, ALLEGRO_KEY_W))
        {
            pong->player1.vy = -PADDLE_SPEED;
        }
        else
        {
            pong->player1.vy = 0;
        }

        if (pong->CPU == false)
        {
            if (al_key_down(state, ALLEGRO_KEY_DOWN))
            {
                pong->player2.vy = PADDLE_SPEED;
            }
            else if (al_key_down(state, ALLEGRO_KEY_UP))
            {
                pong->player2.vy = -PADDLE_SPEED;
            }
            else
            {
                pong->player2.vy = 0;
            }
        }
    }
    else
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER) && pong->state != MENU)
        {
            pong->state = SERVE;
            init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);

            pong->player1_score = 0;
            pong->player2_score = 0;

            if (pong->winning_player == 1)
            {
                pong->serving_player = 2;
            }
            else
            {
                pong->serving_player = 1;
            }
        }else if (al_key_down(state, ALLEGRO_KEY_ESCAPE) && pong->state != MENU)
        {
            pong->state = MENU;
            init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);

            pong->player1_score = 0;
            pong->player2_score = 0;

            if (pong->winning_player == 1)
            {
                pong->serving_player = 2;
            }
            else
            {
                pong->serving_player = 1;
            }
        }
    }
}
void cpu_ia (struct Pong* pong)
{
    float m = ((pong->y2 - pong->y1 ) / (pong->x2 - pong->x1));
    float b = pong->y1 - m*pong->x1;
    float y_cpu = m*pong->player2.x + b;

    if(pong->ball.x > (TABLE_WIDTH/2 + TABLE_WIDTH/4 ) )
    {        
        if ((pong->player2.y+pong->player2.height) > y_cpu && pong->player2.y < y_cpu)
        {
            pong->player2.vy = 0;
        }
        else if (pong->ball.vx <= 0)
        {
            pong->player2.vy = 0;
        }
        else if (y_cpu > (pong->player2.y+pong->player2.height/2))
        {
            pong->player2.vy = PADDLE_SPEED;
        }        
        else if (y_cpu < (pong->player2.y+pong->player2.height/2))
        {
            pong->player2.vy = -PADDLE_SPEED;
        }
    }
    else pong->player2.vy = 0;
}

void update_pong(struct Pong* pong, double dt)
{
    if (pong->state == PLAY)
    {        
        if (pong->CPU)
        {
            if (pong->cycle)
            {
                pong->x1 = pong->ball.x + BALL_SIZE/2;
                pong->y1 = pong->ball.y + BALL_SIZE/2;
            }
            else
            {
                pong->x2 = pong->ball.x + BALL_SIZE/2;
                pong->y2 = pong->ball.y + BALL_SIZE/2;
                cpu_ia(pong);
            }
        }
        update_paddle(&pong->player1, dt);
        update_paddle(&pong->player2, dt);
        update_ball(&pong->ball, dt);

        struct Hitbox ball_hitbox;
        build_ball_hitbox(pong->ball, &ball_hitbox);

        struct Hitbox player1_hitbox;
        build_paddle_hitbox(pong->player1, &player1_hitbox);

        struct Hitbox player2_hitbox;
        build_paddle_hitbox(pong->player2, &player2_hitbox);

        if (ball_hitbox.x1 > TABLE_WIDTH)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player1_score;
            pong->serving_player = 2;

            if (pong->player1_score == MAX_POINTS)
            {
                pong->winning_player = 1;
                pong->state = DONE;
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
            pong->cycle = false;
        }
        else if (ball_hitbox.x2 < 0)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player2_score;
            pong->serving_player = 1;

            if (pong->player2_score == MAX_POINTS)
            {
                pong->winning_player = 2;
                pong->state = DONE;
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
            pong->cycle = false;
        }

        if (ball_hitbox.y1 <= 0)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = 0;
            pong->ball.vy *= -1;
            pong->cycle = false;
        }
        else if (ball_hitbox.y2 >= TABLE_HEIGHT)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = TABLE_HEIGHT - pong->ball.height;
            pong->ball.vy *= -1;
            pong->cycle = false;
        }
        
        if (collides(ball_hitbox, player1_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player1_hitbox.x2;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
        else if (collides(ball_hitbox, player2_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player2_hitbox.x1 - pong->ball.width;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
    }
}

void render_pong(struct Pong pong, struct Fonts fonts)
{
    al_draw_filled_rectangle(
        TABLE_WIDTH / 2 - MID_LINE_WIDTH / 2, 0,
        TABLE_WIDTH / 2 + MID_LINE_WIDTH / 2, TABLE_HEIGHT,
        al_map_rgb(255, 255, 255)
    );
    render_paddle(pong.player1);
    render_paddle(pong.player2);
    render_ball(pong.ball);

    char score[3];
    sprintf(score, "%d", pong.player1_score);
    al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, score);
    sprintf(score, "%d", pong.player2_score);
    al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, score);

    if (pong.state == START)
    {
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to start");
    }
    else if (pong.state == SERVE)
    {
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to serve");
    }
    else if (pong.state == MENU)
    {
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "MENU");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3 + TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "Press 1 to single player");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2 +  TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "Press 2 to coop");
    }
    else if (pong.state == DONE)
    {     
        char winner_message[14];
        if (pong.CPU == false)
        {
            sprintf(winner_message, "Player %d won!", pong.winning_player);
        }
        else
        {
            if(pong.winning_player == 2)
            {
                al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "Cpu won!");
            }
            else
            {
                sprintf(winner_message, "Player %d won!", pong.winning_player);
            }                
        }        
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, winner_message);
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to restart");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2 + TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "Press escape to back menu");
    }
}