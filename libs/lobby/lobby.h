#ifndef LOBBY_H
#define LOBBY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifdef __unix__
    #define OS_WINDOWS 0
    #include <unistd.h>
#elif defined(_WIN32)
    #define OS_WINDOWS 1
    #include <windows.h>
#endif
#include "../player/player.h"
#include "../core/constants.h"
#include "../cards/suit.h"

typedef enum GameType 
{
    TWO_PLAYERS,
    THREE_PLAYERS,
    FOUR_PLAYERS
} GameType;

typedef struct Lobby
{
    uchar name[MAX_GAME_NAME];
    GameType type;
    SuitType trump_suit;
    Player *players[MAX_PLAYERS];
    Card *deck[ALL_CARDS];
    uchar size_of_cards;
} Lobby;

uchar get_players_size(Lobby *lobby);
void print_deck(Card *deck[], uchar size);
void deal_cards(Lobby *lobby);
void shuffle_cards(Lobby *lobby);
void destroy_lobby(Lobby *lobby);
void sort_players_based_on_valuable_index(Lobby *lobby, uchar valueable_index);
void specify_elder_player(Lobby *lobby);
void setup_deck(Lobby *lobby);
void create_lobby(Lobby *lobby, char name[], GameType type);
void play(Lobby *lobby);


#endif