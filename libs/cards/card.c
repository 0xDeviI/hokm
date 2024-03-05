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
    uchar *cardString = (uchar *) malloc(MAX_CARD_ASCII_IMAGE_LENGTH);
    uchar *letter = get_card_letter_character(card);
    uchar *suit = get_card_suit_character(card);
    if (card->isAce || card->isFace)
        snprintf(cardString, MAX_CARD_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%s        │\n|         |\n│    %s    |\n|         |\n│        %s│\n└─────────┘", 
        letter, suit, letter);
    else
        if (card->number == 10)
            snprintf(cardString, MAX_CARD_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%d       │\n|         |\n│    %s    |\n|         |\n│       %d│\n└─────────┘", 
            card->number, suit, card->number);
        else
            snprintf(cardString, MAX_CARD_ASCII_IMAGE_LENGTH, "┌─────────┐\n│%d        │\n|         |\n│    %s    |\n|         |\n│        %d│\n└─────────┘", 
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