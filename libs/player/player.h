#ifndef PLAYER_H
#define PLAYER_H

#include "../core/globals.h"
#include "../core/constants.h"
#include "../cards/card.h"

typedef enum PlayerType {
    HUMAN,
    AI
} PlayerType;

typedef struct Player
{
    uchar name[MAX_PLAYER_NAME];
    Card *cards[MAX_CARDS_PER_HAND];
    PlayerType type;
    uchar size_of_cards;
} Player;


#endif