/**
 * @file pongnetworking.c
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief pong game network code
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "messages.h"
#include "utilities.h"
#include "pongnetworking.h"
#include "pong.h"

int accept_player(int server_socket, char side) {
    int player_socket;
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);
    do {
        player_socket = accept_client(server_socket);
        if (recv(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
            perror("recv()");
            free(buffer);
            return -1;
        }
    } while (buffer[0] != REQUEST_CONNECT);
    buffer[0] = CONFIRM_CONNECT;
    buffer[1] = side;
    
    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }
    free(buffer);

    return player_socket;
}

int update_player_position(player_t* player, int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = GET_POSITION;
    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    if (recv(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("recv()");
        free(buffer);
        return -1;
    }

    player->paddle.position.x = (float_from_bytes(buffer + 1) * 1280) / 30;
    player->paddle.position.y = (float_from_bytes(buffer + 5) * 720) / 20;

    free(buffer);
    return 0;    
}

int send_player_position(player_t player, int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = SET_POSITION;
    memcpy(buffer + 1, float_to_bytes((player.paddle.position.x * 30) / 1280), 4);
    memcpy(buffer + 5, float_to_bytes((player.paddle.position.y * 20) / 720), 4);

    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;    
}

int send_ball_position(ball_t ball, int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = SET_BALL_POSITION;
    memcpy(buffer + 1, float_to_bytes((ball.position.x * 30) / 1280), 4);
    memcpy(buffer + 5, float_to_bytes((ball.position.y * 20) / 720), 4);

    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int send_score_info(unsigned char* score, int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = SET_SCORE;
    buffer[1] = score[0];
    buffer[2] = score[1];

    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int send_start_game(int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = START_GAME;

    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int send_game_over(int player_socket) {
    unsigned char* buffer = malloc(sizeof(unsigned char) * MESSAGE_LENGTH);

    buffer[0] = GAME_OVER;

    if (send(player_socket, buffer, MESSAGE_LENGTH, 0) < 0) {
        perror("send()");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}