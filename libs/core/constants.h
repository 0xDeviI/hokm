#ifndef CONSTANTS_H
#define CONSTANTS_H

// ########### Lobby Constants ###########
#define MAX_GAME_NAME 32

// ########### Player Constants ###########
#define MAX_PLAYER_NAME 32
#define MAX_PLAYERS 4

// ########### Cards Constants ###########
#define ALL_CARDS 52
#define MAX_CARDS_PER_HAND 17
// #define CARD_INVALID_FLAG 0xA0
#define DECK_SHUFFLE_DEPTH 1
#define MAX_CARD_ASCII_IMAGE_LENGTH 160
#define MAX_CARDS_PER_ROW 5
#define CARD_ASCII_IMAGE_HEIGHT 7

// ########### Menu Constants ###########
#define MAX_MENU_OPTION_LENGTH 64
#define MAX_MENU_OPTIONS_PER_MENU 16

// ########### Memory Logic Constants ###########
#define MAX_STRING_BUFFER_LENGTH 32

// ########### Screen Constants ###########
// The game could run on 2K resolution monitor at its max size: 2560x1440
#define SCREEN_MAX_COLUMNS 2560
#define SCREEN_MAX_ROWS 1440
// #define SCREEN_FRAME_USE_DELAYED_REFRESH_RATE 0
// #define SCREEN_FRAME_REFRESH_RATE_MS 100
#define SCREEN_EMPTY_SPACE_CHARACTER ' '

// ########### Multi-tasking Constants ###########
#define MAX_PARALLEL_THREADS 1024

#endif