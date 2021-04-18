/**
 * @file pong.c
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Pong game logic
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pong.h"

#define FIELD_WIDTH 1280
#define FIELD_HEIGHT 720

#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 150

#define BALL_WIDTH 30
#define BALL_HEIGHT 30

void init_game(player_t* player_l, player_t* player_r, ball_t* ball) { 
	ball->position.x = FIELD_WIDTH / 2;
	ball->position.y = FIELD_HEIGHT / 2;
	ball->size.x = BALL_WIDTH;
	ball->size.y = BALL_HEIGHT;
	ball->movement_vector.x = -8;
	ball->movement_vector.y = -8;

	pair_t size;
	size.x = PADDLE_WIDTH;
	size.y = PADDLE_HEIGHT;

	player_l->paddle.size = size;
	player_r->paddle.size = size;

	player_l->paddle.position.x = 0;
	player_l->paddle.position.y = FIELD_HEIGHT / 2;
	player_r->paddle.position.x = FIELD_WIDTH - PADDLE_WIDTH;
	player_r->paddle.position.y = FIELD_HEIGHT / 2;	
}

int check_collision(paddle_t paddle, ball_t ball) {
	float ball_left, ball_right, ball_top, ball_bottom;
	float paddle_left, paddle_right, paddle_top, paddle_bottom;

	ball_left = ball.position.x;
	ball_top = ball.position.y;
	ball_right = ball.position.x + BALL_WIDTH;
	ball_bottom = ball.position.y + BALL_HEIGHT;

	paddle_left = paddle.position.x;
	paddle_top = paddle.position.y;
	paddle_right = paddle.position.x + PADDLE_WIDTH;
	paddle_bottom = paddle.position.y + PADDLE_HEIGHT;

	if (ball_left > paddle_right) {
		return 0;
	}

	if (ball_right < paddle_left) {
		return 0;
	}

	if (ball_top > paddle_bottom) {
		return 0;
	}

	if (ball_bottom < paddle_top) {
		return 0;
	}

	return 1;
}

void move_ball(ball_t* ball, player_t* player_l, player_t* player_r, unsigned char* score) {
	ball->position.x += ball->movement_vector.x;
	ball->position.y += ball->movement_vector.y;

	if (ball->position.x < 0) {
		score[1] += 1;
		init_game(player_l, player_r, ball);
	}

	if (ball->position.x - BALL_WIDTH > FIELD_WIDTH) {
		score[0] += 1;
		init_game(player_l, player_r, ball);
	}

	if (ball->position.y < 0 || ball->position.y + BALL_HEIGHT > FIELD_HEIGHT - 20){
		ball->movement_vector.y = -ball->movement_vector.y;
	}

	if (check_collision(player_r->paddle, *ball)){
		ball->movement_vector.x = -ball->movement_vector.x;
		paddle_t paddle = player_r->paddle;
		float dy;
		float collision_pos = (paddle.position.y + paddle.size.y) - ball->position.y;

		if (collision_pos < PADDLE_HEIGHT / 2) {
			dy = 8.0 - ((collision_pos / (PADDLE_HEIGHT / 2)) * 8.0);
		} else {
			collision_pos -= (PADDLE_HEIGHT / 2);
			dy = (collision_pos / (PADDLE_HEIGHT / 2)) * 8.0;
		}

		ball->movement_vector.y = dy;

		if (ball->position.x > FIELD_WIDTH - 40) ball->position.x = FIELD_WIDTH - 40;  
	}

	if(check_collision(player_l->paddle, *ball)){
		ball->movement_vector.x = -ball->movement_vector.x;
		paddle_t paddle = player_l->paddle;
		float dy;
		float collision_pos = (paddle.position.y + paddle.size.y) - ball->position.y;

		if (collision_pos < PADDLE_HEIGHT / 2) {
			dy = 8.0 - ((collision_pos / (PADDLE_HEIGHT / 2)) * 8.0);
		} else {
			collision_pos -= (PADDLE_HEIGHT / 2);
			dy = (collision_pos / (PADDLE_HEIGHT / 2)) * 8.0;
		}

		ball->movement_vector.y = dy;

		if (ball->position.x < 40) ball->position.x = 40;
	}
}