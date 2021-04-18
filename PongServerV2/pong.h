/**
 * @file pong.h
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Contains pong game structures and game engine functions definitions
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PONG_H
#define PONG_H

/**
 * @brief Defines a point structure for two dimensional space
 * 
 */
typedef struct pair_s {
	float x;
	float y;
} pair_t;

/**
 * @brief Defines paddle structure
 * 
 */
typedef struct paddle_s {
	pair_t position;
	pair_t size;
} paddle_t;

/**
 * @brief Defines ball structure
 * 
 */
typedef struct ball_s {
	pair_t position;
	pair_t size;
	pair_t movement_vector;
} ball_t;

/**
 * @brief Defines player structure
 * 
 */
typedef struct player_s {
	paddle_t paddle;
} player_t;

/**
 * @brief Initialize new game
 * 
 * @param player_l Left player structure
 * @param player_r Right player structure
 * @param ball Ball structue
 */
void init_game(player_t* player_l, player_t* player_r, ball_t* ball);

/**
 * @brief Move ball, check for collisions and update score
 * 
 * @param ball Ball structure
 * @param player_l Left player structure
 * @param player_r Right player Structure
 * @param score Score to update
 */
void move_ball(ball_t* ball, player_t* player_l, player_t* player_r, unsigned char* score);

#endif