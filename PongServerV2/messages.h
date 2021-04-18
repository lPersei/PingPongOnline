/**
 * @file messages.h
 * @author Oleh Shatskyi & Hryhorii Shynkovskyi
 * @brief Message headers definition
 * @version 1.0
 * @date 2019-09-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MESSAGES_H
#define MESSAGES_H

/**
 * @brief Request connection message 
 * Client sends this message to request connection
 * 
 */
#define REQUEST_CONNECT 65

/**
 * @brief Confirm connection message
 * Server confirms connection with this message
 */
#define CONFIRM_CONNECT 66

/**
 * @brief Left side message
 * Server sends this if client connected first 
 */
#define SIDE_LEFT 67

/**
 * @brief Right side message
 * Server sends this message if client connected second
 */
#define SIDE_RIGHT 68

/**
 * @brief Start game message
 * Server sends this to infor clients that game has started
 */
#define START_GAME 69

/**
 * @brief Get position message
 * Server asks clients for position with this message
 */
#define GET_POSITION 70

/**
 * @brief Set postion message
 * Used to update positions both in server and client
 */
#define SET_POSITION 71

/**
 * @brief Set ball position message
 * Server sends ball position to clients with this message
 */
#define SET_BALL_POSITION 72

/**
 * @brief Set score message
 * Server sends score with this message
 */
#define SET_SCORE 73

/**
 * @brief Game over message
 * Server finishes game with this message
 */
#define GAME_OVER 74

/**
 * @brief Message length constant
 * 
 */
#define MSG_LENGTH 9

#endif