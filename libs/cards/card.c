/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Wednesday, February 14th 2024, 3:15:25 am
 * Author: Armin Asefi
 * 
 * This license agreement (the "License") is a legal agreement between 
 * you (an individual or entity) and Armin Asefi (the "Licensor") 
 * regarding the use of the software provided by the Licensor.
 * 
 * By accessing, viewing, or sharing the source code of the software
 * provided by the Licensor, you agree to be bound by the terms and 
 * conditions of this License.
 * 
 * 1. GRANT OF LICENSE
 * SUBJECT TO THE TERMS AND CONDITIONS OF THIS LICENSE, THE LICENSOR 
 * GRANTS YOU A NON-EXCLUSIVE, NON-TRANSFERABLE, ROYALTY-FREE LICENSE
 * TO ACCESS, VIEW, AND SHARE THE SOURCE CODE OF THE SOFTWARE PROVIDED
 * BY THE LICENSOR.
 * 
 * 2. RESTRICTIONS
 * YOU ARE NOT ALLOWED TO MODIFY, DISTRIBUTE, SUBLICENSE, OR USE THE
 * SOURCE CODE FOR ANY COMMERCIAL PURPOSES WITHOUT THE EXPLICIT WRITTEN
 * PERMISSION OF THE LICENSOR. YOU MAY NOT REVERSE ENGINEER, DECOMPILE,
 * OR DISASSEMBLE THE SOFTWARE.
 * 
 * 3. OWNERSHIP
 * THE LICENSOR RETAINS ALL INTELLECTUAL PROPERTY RIGHTS IN THE SOFTWARE
 * AND THE SOURCE CODE. THIS LICENSE DOES NOT GRANT YOU ANY RIGHTS TO 
 * PATENTS, COPYRIGHTS, TRADE SECRETS, OR ANY OTHER INTELLECTUAL PROPERTY
 * OWNED BY THE LICENSOR.
 * 
 * 4. DISCLAIMER OF WARRANTY
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT ANY WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED. THE LICENSOR SHALL NOT BE LIABLE FOR ANY DAMAGES
 * ARISING OUT OF THE USE OR INABILITY TO USE THE SOFTWARE.
 * 
 * 5. TERMINATION
 * THIS LICENSE IS EFFECTIVE UNTIL TERMINATED. THE LICENSOR RESERVES THE
 * RIGHT TO TERMINATE THIS LICENSE AT ANY TIME IF YOU FAIL TO COMPLY WITH
 * ITS TERMS AND CONDITIONS. UPON TERMINATION, YOU MUST CEASE ALL USE OF
 * THE SOFTWARE AND DESTROY ALL COPIES OF THE SOURCE CODE.
 * 
 * 6. GOVERNING LAW
 * THIS LICENSE SHALL BE GOVERNED BY AND CONSTRUED IN ACCORDANCE WITH THE
 * LAWS OF --IRAN, ISLAMIC REPUBLIC--. ANY DISPUTES ARISING UNDER THIS LICENSE
 * SHALL BE SUBJECT TO THE EXCLUSIVE JURISDICTION OF THE COURTS IN --IRAN, ISLAMIC REPUBLIC--.
 * 
 */


#include "./card.h"


Card *get_valuable_card(Card *card1, Card *card2) {
    // Check for trumps
    if (card1->suit.isTrump && !card2->suit.isTrump)
        return card1;
    else if (!card1->suit.isTrump && card2->suit.isTrump)
        return card2;
    else {
        // Check for aces
        if (card1->isAce && !card2->isAce)
            return card1;
        else if (!card1->isAce && card2->isAce)
            return card2;
        else {
            // Check for faces
            if (card1->isFace && !card2->isFace)
                return card1;
            else if (!card1->isFace && card2->isFace)
                return card2;
            else if (card1->isFace && card2->isFace)
                return card1->face > card2->face ? card1 : card2;
            else
                return card1->number > card2->number ? card1 : card2;
        }
    }
}


uchar get_elder_player(Card *deck[], uchar size_of_players) {
    // Most valuable card is lowest one by default, we edit it if we could find more valuable card.
    Card most_valuable_card;
    Suit most_valuable_card_suit;
    most_valuable_card.isFace = 0;
    most_valuable_card.number = 2;
    most_valuable_card.isAce = 0;
    most_valuable_card_suit.isTrump = 0;
    most_valuable_card_suit.type = SPADE;
    most_valuable_card.suit = most_valuable_card_suit;

    uchar highest_player_index = 0;
    for (uchar i = 0; i < size_of_players; i++) {
        Card *valuable_card = get_valuable_card(deck[i], &most_valuable_card);
        if (valuable_card != &most_valuable_card) {
            most_valuable_card = *valuable_card;
            highest_player_index = i;
        }
    }
    return highest_player_index;
}


uchar *get_card_suit_character(Card *card) {
    switch (card->suit.type)
    {
    case CLUB:
        return "♣";
    case SPADE:
        return "♠";
    case DIAMOND:
        return "◆";
    default:
        return "❤️";
    }
}


uchar *get_card_letter_character(Card *card) {
    if (card->isAce)
        return "A";

    switch (card->face)
    {
    case KING:
        return "K";
    case QUEEN:
        return "Q";
    default:
        return "J";
    }
}


uchar *get_card_ascii_image(Card *card) {
    uchar *cardString = (uchar *) malloc(CARDS_MAX_ASCII_IMAGE_LENGTH);
    uchar *letter = get_card_letter_character(card);
    uchar *suit = get_card_suit_character(card);
    if (card->isAce || card->isFace)
        snprintf(cardString, CARDS_MAX_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%s        │\n|         |\n│    %s    |\n|         |\n│        %s│\n└─────────┘", 
        letter, suit, letter);
    else
        if (card->number == 10)
            snprintf(cardString, CARDS_MAX_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%d       │\n|         |\n│    %s    |\n|         |\n│       %d│\n└─────────┘", 
            card->number, suit, card->number);
        else
            snprintf(cardString, CARDS_MAX_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%d        │\n|         |\n│    %s    |\n|         |\n│        %d│\n└─────────┘", 
            card->number, suit, card->number);
    return cardString;
}


void print_card(Card *card) {
    // old method: String construction
    // uchar suit;
    // uchar face;
    // uchar number;

    // switch (card->suit.type)
    // {
    //     case HEART:
    //         suit = 'H';
    //         break;
    //     case DIAMOND:
    //         suit = 'D';
    //        break;
    //     case SPADE:
    //         suit = 'S';
    //         break;
    //     case CLUB:
    //         suit = 'C';
    //         break;
    // }

    // if (card->isFace) {
    //     switch (card->face)
    //     {
    //     case KING:
    //     face = 'K';
    //         break;
    //     case QUEEN:
    //     face = 'Q';
    //         break;
    //     case JACK:
    //     face = 'J';
    //         break;
    //     }
    // }

    // if (card->isAce)
    //     number = 'A';
    // else
    //     number = card->number;

    // if (card->isAce) {
    //     printf("%s(%c) %c (%c)\n", card->suit.isTrump ? "* " : "  ", number, suit, number);
    // }
    // else if (card->isFace) {
    //     printf("%s(%c) %c (%c)\n", card->suit.isTrump ? "* " : "  ", face, suit, face);
    // }
    // else {
    //     printf("%s(%d) %c (%d)\n", card->suit.isTrump ? "* " : "  ", number, suit, number);
    // }

    // new method: TUI
    printf("%s\n", card->card_ascii_image);
}