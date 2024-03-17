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


#include "./lobby.h"


uchar get_players_size(Lobby *lobby) {
    return lobby->type == TWO_PLAYERS ? 2 : lobby->type == THREE_PLAYERS ? 3 : 4;
}


void deal_cards(Lobby *lobby) {
    switch (lobby->type)
    {
    case TWO_PLAYERS:
        break;
    case THREE_PLAYERS:
        break;
    case FOUR_PLAYERS:
        for (uchar i = 0; i < 4; i++) {
            uchar index;
            for (uchar j = 0; j < 5; j++) {
                index = CARDS_AVAILABLE - lobby->size_of_cards;
                // If player is not elder player, we should check that is it a trump card or not?
                if (i != 0)
                    lobby->deck[index]->suit.isTrump = lobby->deck[index]->suit.type == lobby->trump_suit ? 1 : 0;
                lobby->players[i]->cards[lobby->players[i]->size_of_cards++] = lobby->deck[index];
                lobby->size_of_cards -= 1;
            }
            if (i == 0) {
                // Printing deck to elder player
                print_deck(lobby->players[i]->cards, lobby->players[i]->size_of_cards);
                
                // Asking elder player to specify trump suit
                uchar suit;
                printf("Select trump suit (H, D, S, C): ");
                suit = getchar();
                while (getchar() != '\n');
                suit += 32;

                // Validating input
                while (suit != 'h' && suit != 'd' && suit != 's' && suit != 'c') {
                    printf("Select trump suit (H, D, S, C): ");
                    suit = getchar();
                    while (getchar() != '\n');
                    suit += 32;
                }

                // Setting trump suit for lobby object
                switch (suit)
                {
                case 'h':
                lobby->trump_suit = HEART;
                    break;
                case 'd':
                lobby->trump_suit = DIAMOND;
                    break;
                case 's':
                lobby->trump_suit = SPADE;
                    break;
                case 'c':
                lobby->trump_suit = CLUB;
                    break;
                }

                // Updating elder cards to be trump if they really are
                for (uchar j = 0; j < lobby->players[i]->size_of_cards; j++)
                    if (lobby->players[i]->cards[j]->suit.type == lobby->trump_suit)
                        lobby->players[i]->cards[j]->suit.isTrump = 1;
            }
        }

        while (lobby->size_of_cards > 0) {
            for (uchar i = 0; i < 4; i++) {
                uchar index;
                for (uchar j = 0; j < 4; j++) {
                    index = CARDS_AVAILABLE - lobby->size_of_cards;
                    // Checking for trump card
                    lobby->deck[index]->suit.isTrump = lobby->deck[index]->suit.type == lobby->trump_suit ? 1 : 0;
                    lobby->players[i]->cards[lobby->players[i]->size_of_cards++] = lobby->deck[index];
                    lobby->size_of_cards -= 1;
                }
            }
        }
        break;
    }
}


void shuffle_cards(Lobby *lobby) {
    // unsigned int seed = (unsigned int)time(NULL);
    // it should be used like:
    // int sIndex = rand_r(&seed) % lobby->size_of_cards;

    for (uchar i = 0; i < CARDS_DECK_SHUFFLE_DEPTH; i++) {
        srand(time(NULL));
        for (uchar j = 0; j < lobby->size_of_cards; j++) {
            int sIndex = rand() % lobby->size_of_cards;
            Card *fCard = lobby->deck[j];
            Card *sCard = lobby->deck[sIndex];
            lobby->deck[j] = sCard;
            lobby->deck[sIndex] = fCard;
        }
        sleep(2);
    }
}


void destroy_lobby(Lobby *lobby) {
    uchar size_of_players = get_players_size(lobby);
    for (uchar i = 0; i < size_of_players; i++) {
        free(lobby->players[i]);
        lobby->players[i] = NULL;
    }

    for (uchar i = 0; i < lobby->size_of_cards; i++) {
        free(lobby->deck[i]);
        lobby->deck[i] = NULL;
    }
}


void print_deck(Card *deck[], uchar size) {
    // We use line-by-line logic; means that we start to iterate for a range of ASCII_IMAGE_HEIGHT. Then we iterate on all
    // cards and add n'th line of its ASCII image to (allocated buffer/output stream).
    // n is iteration of ASCII_IMAGE_HEIGHT and determines which line should be used now.
    uchar current_row = 0;
    for (uchar i = 0; i < size; i += (i + CARDS_PER_ROW > size ? size - i : CARDS_PER_ROW)) {
        for (uchar current_line = 0; current_line < CARDS_MAX_ASCII_IMAGE_HEIGHT; current_line++) {
            uchar bound = (i + CARDS_PER_ROW > size ? size : i + CARDS_PER_ROW);
            for (uchar j = i; j < bound; j++) {
                uchar *card_ascii_image = (uchar *) malloc(CARDS_MAX_ASCII_IMAGE_LENGTH);
                strncpy(card_ascii_image, deck[j]->card_ascii_image, CARDS_MAX_ASCII_IMAGE_LENGTH);
                uchar *line = strtok(card_ascii_image, "\n");
                for (uchar c = 0 ; c < current_line && line != NULL; c++)
                    line = strtok(NULL, "\n");

                printf(line);
                if (((j + 1) % (CARDS_PER_ROW) == 0 && j != 0) || ((i + CARDS_PER_ROW > size && j == bound - 1)))
                    printf("\n");
                free(card_ascii_image);
            }
        }
    }
}


void sort_players_based_on_valuable_index(Lobby *lobby, uchar valueable_index) {
    if (valueable_index != 0) {
        if (lobby->type == TWO_PLAYERS) {

        }
        else if (lobby->type == THREE_PLAYERS) {

        }
        else {
            uchar valueable_pair_index;
            uchar opponent_index;
            uchar opponent_pair_index;
            if (valueable_index >= 2) {
                valueable_pair_index = valueable_index - 2;
                opponent_index = valueable_index - 1;
            }
            else {
                valueable_pair_index = valueable_index + 2;
                opponent_index = valueable_index + 1;
            }

            if (opponent_index >= 2)
                opponent_pair_index = opponent_index - 2;
            else
                opponent_pair_index = opponent_index + 2;

            Player *valueable_player = lobby->players[valueable_index];
            Player *valueable_pair_player = lobby->players[valueable_pair_index];
            Player *opponent_player = lobby->players[opponent_index];
            Player *opponent_pair_player = lobby->players[opponent_pair_index];

            lobby->players[0] = valueable_player;
            lobby->players[1] = valueable_pair_player;
            lobby->players[2] = opponent_player;
            lobby->players[3] = opponent_pair_player;
        }
    }
}


void specify_elder_player(Lobby *lobby) {
    printf("Specifying Elder Player. Please wait ...\n");
    shuffle_cards(lobby);
    

    uchar size_of_players = get_players_size(lobby);
    Card *cards[size_of_players];
    
    for (uchar i = 0; i < size_of_players; i++) {
        cards[i] = lobby->deck[i];
    }

    // Ask to find which card is better
    uchar elder_player_index = get_elder_player(cards, size_of_players);

    // Re-sort players list in lobby
    sort_players_based_on_valuable_index(lobby, elder_player_index);    
}


void setup_deck(Lobby *lobby) {
    lobby->size_of_cards = 0;
    for (uchar i = 0; i < sizeof(SuitType); i++) {
        // Creating 1-10 Cards and their faces for each suit
        uchar index;
        for (uchar j = 0; j < 13; j++) {
            index = i * 13 + j;
            lobby->deck[index] = (Card*) malloc(sizeof(Card));
            lobby->deck[index]->suit.type = (SuitType) i;
            lobby->deck[index]->number = j + 1;
            if (lobby->deck[index]->number == 1)
                lobby->deck[index]->isAce = 1;
            lobby->deck[index]->isFace = 0;
            lobby->size_of_cards += 1;
            
            // Creating King, Queen and Jack cards
            if (j == 12 || j == 11 || j == 10) {
                lobby->deck[index]->isFace = 1;
                switch (j)
                {
                case 10:
                    lobby->deck[index]->face = JACK;
                    break;
                case 11:
                    lobby->deck[index]->face = QUEEN;
                    break;
                case 12:
                    lobby->deck[index]->face = KING;
                    break;
                }
            }

            uchar *card_ascii_image = get_card_ascii_image(lobby->deck[index]);
            strncpy(lobby->deck[index]->card_ascii_image, card_ascii_image, sizeof(lobby->deck[index]->card_ascii_image));
            free(card_ascii_image);
        }
    }

    // specifying eldest player
    // specify_elder_player(lobby);
    // printf("Shuffling cards. Please wait ...\n");
    shuffle_cards(lobby);
    // deal_cards(lobby);

    // printf("Printing cards...\n");

    // uchar size_of_players = get_players_size(lobby);
    // for (uchar i = 0; i < size_of_players; i++) {
    //     printf("Cards in hand of player %d:\n", i + 1);
    //     print_deck(lobby->players[i]->cards, lobby->players[i]->size_of_cards);
    // }
}


void create_lobby(Lobby *lobby, char name[], GameType type) {
    if (strlen(name) < LOBBY_MAX_GAME_NAME)
        strcpy(lobby->name, name);
    else
        strcpy(lobby->name, "Unknown Game");
    lobby->type = type;

    setup_deck(lobby);
}