#ifndef SUIT_H
#define SUIT_H

#include "../core/globals.h"

typedef enum SuitType 
{
    HEART,
    DIAMOND,
    SPADE,
    CLUB
} SuitType;


typedef struct Suit 
{
    SuitType type;
    uchar isTrump;
} Suit;

#endif