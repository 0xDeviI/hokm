#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stdlib.h>
#include "../core/constants.h"
#include "../core/globals.h"
#include "./suit.h"
#include "./face.h"

typedef struct Card
{
    Suit suit;
    Face face;
    uchar isAce;
    uchar isFace;
    uchar number;
    uchar card_ascii_image[MAX_CARD_ASCII_IMAGE_LENGTH];
} Card;


Card *get_valuable_card(Card *card1, Card *card2);
uchar get_elder_player(Card *deck[], uchar size_of_players);
uchar *get_card_suit_character(Card *card);
uchar *get_card_letter_character(Card *card);
uchar *get_card_ascii_image(Card *card);
void print_card(Card *card);

#endif