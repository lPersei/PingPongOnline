/**
 * @file main.c
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Program entry point
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
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "utilities.h"
#include "messages.h"
#include "pongnetworking.h"

#define MAX_QUEUE 16

int main(int argc, char** argv) {
    //parse command line arguments
    if (argc != 3) {
        printf("Usage: ./%s PORT ADDRESS\n", argv[0]);
        return -1;
    }

    int port;
    unsigned int address;
    if (parse_port_and_address(argv[1], &port, argv[2], &address) < 0) {
        fprintf(stderr, "Error parsing port or address");
        return -1;
    }

    //set up server
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socketfd == -1) {
        fprintf(stderr, "ERROR: unable to create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    memset(&(server_address.sin_zero), '\0', 8);

    if ((bind(socketfd, (struct sockaddr *) &server_address, sizeof(struct sockaddr))) == -1) {
        fprintf(stderr, "ERROR: unable to bind port %d: %s\n", port, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (listen(socketfd, MAX_QUEUE) < 0) {
        perror("listen");
        return -1;
    }

    printf("Server is running on port: %s, address: %s\n", argv[1], argv[2]);

    int left_player_socket = accept_player(socketfd, SIDE_LEFT);
    int right_player_socket = accept_player(socketfd, SIDE_RIGHT);

    player_t player_l;
    player_t player_r;
    ball_t ball;
    unsigned char score[] = { 0, 0 };
    clock_t t;
    struct timespec ts;
    ts.tv_sec = 0;

    send_start_game(left_player_socket);
    send_start_game(right_player_socket);

    bool running = true;
    init_game(&player_l, &player_r, &ball);

    while(running) {
        t = clock();
        update_player_position(&player_l, left_player_socket);
        update_player_position(&player_r, right_player_socket);
        send_player_position(player_r, left_player_socket);
        send_player_position(player_l, right_player_socket);
        send_ball_position(ball, left_player_socket);
        send_ball_position(ball, right_player_socket);
        send_score_info(score, left_player_socket);
        send_score_info(score, right_player_socket);
        
        move_ball(&ball, &player_l, &player_r, score);

        if(score[0] > 15) running = false;
        if(score[1] > 15) running = false;

        t = clock() - t;
        
        ts.tv_nsec = 16666666 - (t * 1000);

        nanosleep(&ts, NULL);
    }

    send_game_over(left_player_socket);
    send_game_over(right_player_socket);

    close(left_player_socket);
    close(right_player_socket);

    return 0;
}
