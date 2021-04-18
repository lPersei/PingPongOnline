/**
 * @file utilities.c
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Server utility functions
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "utilities.h"

int accept_client(int server_socket) {
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);

    if((client_socket=accept(server_socket, (struct sockaddr *)&client_address, &len)) < 0){
        perror("accept()");
        return -1;
    }

    printf("Client connected %s\n", inet_ntoa(client_address.sin_addr));
    return client_socket;
}

int parse_port_and_address(char* port_str, int* port_int, char* addr_str, unsigned int* addr_int){
    size_t len = strlen(port_str);
    for (size_t i = 0; i < len; ++i) {
        if (port_str[i] < '0' || port_str[i] > '9') {
			return -1;
		}
    }

    *port_int = atoi(port_str);

    if (*port_int < 0) {
        return -1;
    }
    
    *addr_int = inet_addr(addr_str);

    if (*addr_int < 0) {
        return -1;
    }

    return 0;
}

float float_from_bytes(unsigned char* bytes) {
    uint32_t temp;
    
    temp = ((bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] <<  8) | bytes[0]);
    return *((float *) &temp);
}

unsigned char* float_to_bytes(float f) {
    unsigned char* bytes = malloc(sizeof(unsigned char) * 4);

    unsigned char* temp = (unsigned char*) &f; 
    bytes[0] = temp[0];
    bytes[1] = temp[1];
    bytes[2] = temp[2];
    bytes[3] = temp[3];

    return bytes;
}
