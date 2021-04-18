#ifndef UTILITIES_H
#define UTILITIES_H

#define MESSAGE_LENGTH 9

/**
 * @brief Accepts client connection at server socket
 * 
 * @param server_socket Server socket fd
 * @return int Accepted client fd
 */
int accept_client(int server_socket);

/**
 * @brief Parse port and address from string
 * 
 * @param port_str Port as string input
 * @param port_int Pointer to where to save port number
 * @param addr_str Address as string input
 * @param addr_int Pointer to where to save parsed address
 * @return int 0 if success, -1 if error
 */
int parse_port_and_address(char* port_str, int* port_int, char* addr_str, unsigned int* addr_int);

/**
 * @brief Get float from bytes in network byte order
 * 
 * @param char Bytes array
 * @return float Float from bytes
 */
float float_from_bytes(unsigned char* bytes);

/**
 * @brief Converts float to bytes in network byte order
 * 
 * @param f Float
 * @return unsigned char* pointer to byte array
 */
unsigned char* float_to_bytes(float f);

#endif