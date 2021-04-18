/**
 * @file pongnetworking.h
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Contains networking functions definitions
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PONGNETWORKING_H
#define PONGNETWORKING_H

#include "messages.h"
#include "pong.h"

/**
 * @brief Accept player connecting through listening socket
 * 
 * @param server_socket Server socket
 * @param side Which side player is going to play on
 * @return int Player socket descriptor in case of success, -1 in case of failure
 */
int accept_player(int server_socket, char side);

/**
 * @brief Receive data about position from player
 * 
 * @param player Pointer to player structure to update
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int update_player_position(player_t* player, int player_socket);

/**
 * @brief Send data about player position
 * 
 * @param player Player structure
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int send_player_position(player_t player, int player_socket);

/**
 * @brief Send data about ball position
 * 
 * @param ball Ball structure
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int send_ball_position(ball_t ball, int player_socket);

/**
 * @brief Send data about score
 * 
 * @param score Score
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int send_score_info(unsigned char* score, int player_socket);

/**
 * @brief Send start game message
 * 
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int send_start_game(int player_socket);

/**
 * @brief Send game over message
 * 
 * @param player_socket Player's socket descriptor
 * @return int Status, 0 if OK, -1 if not
 */
int send_game_over(int player_socket);
#endif